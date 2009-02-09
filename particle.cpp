/*
 *  The Mana World
 *  Copyright 2006 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id: particle.cpp 4043 2008-04-07 08:37:23Z b_lindeijer $
 */

#include <algorithm>
#include <cmath>

#include "particle.h"

#include "animationparticle.h"
#include "configuration.h"
#include "imageparticle.h"
#include "log.h"
#include "map.h"
#include "particleemitter.h"
#include "textparticle.h"

#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/fastsqrt.h"
#include "utils/xml.h"

#define SIN45 0.707106781f

class Graphics;
class Image;

int Particle::particleCount = 0;
int Particle::maxCount = 0;
int Particle::fastPhysics = 0;
int Particle::emitterSkip = 1;
const float Particle::PARTICLE_SKY = 800.0f;

Particle::Particle(Map *map):
    mAlive(true),
    mLifetimeLeft(-1),
    mLifetimePast(0),
    mFadeOut(0),
    mFadeIn(0),
    mAutoDelete(true),
    mMap(map),
    mGravity(0.0f),
    mRandomnes(0),
    mBounce(0.0f),
    mTarget(NULL),
    mAcceleration(0.0f),
    mInvDieDistance(-1.0f),
    mMomentum(1.0f)
{
    Particle::particleCount++;
    if (mMap) setSpriteIterator(mMap->addSprite(this));
}


void
Particle::setupEngine()
{
    Particle::maxCount = (int)config.getValue("particleMaxCount", 3000);
    Particle::fastPhysics = (int)config.getValue("particleFastPhysics", 0);
    Particle::emitterSkip = (int)config.getValue("particleEmitterSkip", 0) + 1;
    disableAutoDelete();
    logger->log("Particle engine set up");
}

void Particle::draw(Graphics *, int, int) const {}

bool
Particle::update()
{
    if (!mMap) return false;

    if (mLifetimeLeft == 0)
    {
        mAlive = false;
    }

    if (mAlive)
    {
        //calculate particle movement
        if (mMomentum != 1.0f)
        {
            mVelocity *= mMomentum;
        }

        if (mTarget && mAcceleration != 0.0f)
        {
            Vector dist = mPos - mTarget->getPosition();
            dist.x *= SIN45;
            float invHypotenuse;

            switch (Particle::fastPhysics)
            {
                case 1:
                    invHypotenuse = fastInvSqrt(
                        dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
                    break;
                case 2:
                    invHypotenuse = 2.0f /
                        fabs(dist.x) + fabs(dist.y) + fabs(dist.z);
                    break;
                default:
                    invHypotenuse = 1.0f / sqrt(
                        dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
                    break;
            }

            if (invHypotenuse)
            {
                if (mInvDieDistance > 0.0f && invHypotenuse > mInvDieDistance)
                {
                    mAlive = false;
                }
                float accFactor = invHypotenuse * mAcceleration;
                mVelocity -= dist * accFactor;
            }
        }

        if (mRandomnes > 0)
        {
            mVelocity.x += (rand()%mRandomnes - rand()%mRandomnes) / 1000.0f;
            mVelocity.y += (rand()%mRandomnes - rand()%mRandomnes) / 1000.0f;
            mVelocity.z += (rand()%mRandomnes - rand()%mRandomnes) / 1000.0f;
        }

        mVelocity.z -= mGravity;

        // Update position
        mPos.x += mVelocity.x;
        mPos.y += mVelocity.y * SIN45;
        mPos.z += mVelocity.z * SIN45;

        // Update other stuff
        if (mLifetimeLeft > 0)
        {
            mLifetimeLeft--;
        }
        mLifetimePast++;

        if (mPos.z > PARTICLE_SKY || mPos.z < 0.0f)
        {
            if (mBounce > 0.0f)
            {
                mPos.z *= -mBounce;
                mVelocity *= mBounce;
                mVelocity.z = -mVelocity.z;
            }
            else {
                mAlive = false;
            }
        }

        // Update child emitters
        if (mLifetimePast%Particle::emitterSkip == 0)
        {
            for (   EmitterIterator e = mChildEmitters.begin();
                    e != mChildEmitters.end();
                    e++
                )
            {
                Particles newParticles = (*e)->createParticles();
                for (   ParticleIterator p = newParticles.begin();
                        p != newParticles.end();
                        p++
                    )
                {
                    (*p)->moveBy(mPos.x, mPos.y, mPos.z);
                    mChildParticles.push_back (*p);
                }
            }
        }
    }

    // Update child particles
    for (ParticleIterator p = mChildParticles.begin();
         p != mChildParticles.end();)
    {
        if ((*p)->update())
        {
            p++;
        } else {
            delete (*p);
            p = mChildParticles.erase(p);
        }
    }

    if (!mAlive && mChildParticles.empty() && mAutoDelete)
    {
        return false;
    }

    return true;
}

Particle*
Particle::addEffect(const std::string &particleEffectFile,
                    int pixelX, int pixelY, int rotation)
{
    Particle *newParticle = NULL;

    XML::Document doc(particleEffectFile);
    xmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "effect"))
    {
        logger->log("Error loading particle: %s", particleEffectFile.c_str());
        return NULL;
    }

    ResourceManager *resman = ResourceManager::getInstance();

    // Parse particles
    for_each_xml_child_node(effectChildNode, rootNode)
    {
        // We're only interested in particles
        if (!xmlStrEqual(effectChildNode->name, BAD_CAST "particle"))
            continue;

        // Determine the exact particle type
        xmlNodePtr node;

        // Animation
        if ((node = XML::findFirstChildByName(
                        effectChildNode, "animation"))) {
            newParticle = new AnimationParticle(mMap, node);
        }
        // Image
        else if ((node = XML::findFirstChildByName(
                        effectChildNode, "image"))) {
            Image *img= resman->getImage((const char*)
                    node->xmlChildrenNode->content);

            newParticle = new ImageParticle(mMap, img);
        }
        // Other
        else {
            newParticle = new Particle(mMap);
        }

        // Read and set the basic properties of the particle
        int offsetX = XML::getProperty(effectChildNode, "position-x", 0);
        int offsetY = XML::getProperty(effectChildNode, "position-y", 0);
        int offsetZ = XML::getProperty(effectChildNode, "position-z", 0);

        int particleX = (int) mPos.x + pixelX + offsetX;
        int particleY = (int) mPos.y + pixelY + offsetY;
        int particleZ = (int) mPos.z          + offsetZ;

        int lifetime = XML::getProperty(effectChildNode, "lifetime", -1);

        newParticle->setPosition(particleX, particleY, particleZ);
        newParticle->setLifetime(lifetime);

        // Look for additional emitters for this particle
        for_each_xml_child_node(emitterNode, effectChildNode)
        {
            if (!xmlStrEqual(emitterNode->name, BAD_CAST "emitter"))
                continue;

            ParticleEmitter *newEmitter;
            newEmitter = new ParticleEmitter(emitterNode, newParticle, mMap, rotation);
            newParticle->addEmitter(newEmitter);
        }

        mChildParticles.push_back(newParticle);
    }

    return newParticle;
}


Particle*
Particle::addTextSplashEffect(const std::string &text,
                              int colorR, int colorG, int colorB,
                              gcn::Font *font, int x, int y)
{
    Particle *newParticle = new TextParticle(mMap, text, colorR, colorG, colorB,
                                             font);
    newParticle->setPosition(x, y, 0);
    newParticle->setVelocity(((rand() % 100) - 50) / 200.0f,    // X
                             ((rand() % 100) - 50) / 200.0f,    // Y
                             ((rand() % 100) / 200.0f) + 4.0f); // Z
    newParticle->setGravity(0.1f);
    newParticle->setBounce(0.5f);
    newParticle->setLifetime(200);
    newParticle->setFadeOut(100);

    mChildParticles.push_back(newParticle);

    return newParticle;
}

Particle*
Particle::addTextRiseFadeOutEffect(const std::string &text,
                                   int colorR, int colorG, int colorB,
                                   gcn::Font *font,
                                   int x, int y)
{
    Particle *newParticle = new TextParticle(mMap, text, colorR, colorG, colorB, font);
    newParticle->setPosition(x, y, 0);
    newParticle->setVelocity(0.0f, 0.0f, 0.5f);
    newParticle->setGravity(0.0015f);
    newParticle->setLifetime(300);
    newParticle->setFadeOut(50);
    newParticle->setFadeIn(200);

    mChildParticles.push_back(newParticle);

    return newParticle;
}

void
Particle::setMap(Map *map)
{
    mMap = map;
    if (mMap) setSpriteIterator(mMap->addSprite(this));
}


Particle::~Particle()
{
    // Remove from map sprite list
    if (mMap) mMap->removeSprite(mSpriteIterator);
    // Delete child emitters and child particles
    clear();
    Particle::particleCount--;
}


void
Particle::clear()
{
    std::for_each(mChildEmitters.begin(), mChildEmitters.end(),
            make_dtor(mChildEmitters));
    mChildEmitters.clear();

    std::for_each(mChildParticles.begin(), mChildParticles.end(),
            make_dtor(mChildParticles));
    mChildParticles.clear();
}
