# makemake.rb
# igneus 22.10.2008
#
# ruby script generating Makefile for partriclEd (it should make it easier to
# add new modules into the project)

##########################################################################
# AREA OF USER-MODIFIED DATA
##########################################################################

# List of places where compiler should look for header files for libraries
# SDL, libxml2, physFS, guichan;
# these are just examples: I have SDL and libxml2 in /usr/include/...,
# physFS and guichan are under my home dir: /home/igneus/dist/TMW/include
include = ['/usr/include/SDL', 
           '/usr/include/libxml2', 
           '/usr/local/include']
# flags for the compiler; you shouldn't need to modify them
flags = '-DTMW_DATADIR=\""."\"' #+ ' -g'

##########################################################################
# END OF AREA
##########################################################################

DEBUG=true

def debug(&block)
  if DEBUG then
    block.call
  end
end

class String
  def cpp
    self+'.cpp'
  end

  def h
    self+'.h'
  end

  def o
    self+'.o'
  end
end

compiler = 'g++'

app = 'particled' # name of the executable which should be created

makefile = 'Makefile'

libraries = ['SDL', 
             'SDL_image', 
             'xml2', 
             'physfs',
             'guichan',
             'guichan_sdl']

modules = ['main',
           'log',
           'graphics',
           'map',
           'particle',
           'imageparticle',
           'animationparticle',
           'textparticle',
           'particleemitter',
           'configuration',
           'simpleanimation',
           'resources/resourcemanager',
           'resources/resource',
           'resources/image',
           'resources/imageset',
           'resources/imageloader',
           'resources/dye',
           'resources/spritedef',
           'resources/action',
           'resources/animation',
           'utils/xml',
           'utils/base64',
           'utils/sha256']

File.open(makefile, 'w') do |fw|
  fw.puts "# makefile for TMW particlEd generated by makemake.rb"
  fw.puts "# Don't modify this makefile, but makemake.rb"
  fw.puts "# then recreate makefile by 'ruby makemake.rb'"

  # all rule
  fw.puts "all: "+app

  fw.puts

  # clean rule
  fw.puts 'clean:'
  fw.puts "\trm *.o"
  fw.puts "\trm "+app

  fw.puts

  # objects:
  # include flags are in a variable - it's my convention for readability
  # of the makefile
  is = (include.collect {|i| '-I'+i}).join(' ')
  fw.puts 'INCS='+is
  fw.puts

  modules.each do |m|
    # depends on the .cpp source file
    fw.print File.basename(m).o+': '+m.cpp+' '
    # and on all locally included header files
    IO.popen('grep "#include" '+m.cpp) do |pipe|
      while l = pipe.gets do
        if l =~ /^#include "(.+)"/ then
          fname = File.dirname(m)+'/'+$1
          unless File.exist?(fname) then
            STDERR.puts "warning: file '#{fname}' required by module '#{m}' not found."
            next
          end

          fw.print fname+' '
        end
      end
    end
    fw.puts
    fw.puts "\t"+compiler+' '+flags+' $(INCS) -c '+m.cpp
    fw.puts
  end

  # rule for app
  fw.puts app+': '+(modules.collect {|m| File.basename(m).o}).join(' ')
  fw.puts "\t"+compiler+' '+flags+' '+(libraries.collect {|l| '-l'+l}).join(' ')+' '+(modules.collect {|m| File.basename(m).o}).join(' ')+' -o '+app
end
