#!/bin/env ruby -w

# pe_runner.rb
# igneus 10.2.2009

# simple wrapper for particlEd;
# has simple feature to choose effect
#
# option -r chooses random effect

# modify this to fit 
tmw_data_dir = '/home/igneus/src/cplusplus/particlEd/trunk'


choose = true
# commandline options:
if ARGV.join(' ') =~ /-r/ then
  choose = false
end

effect_subdir = 'graphics/particles'
effects = Dir[tmw_data_dir+'/'+effect_subdir+'/*.particle.xml'].collect do |f|
  File.basename f
end

if choose then
  effects.each_with_index {|f,i| puts "#{i.to_s.rjust(3)}| #{f}"}
  choice = nil
  begin
    print "choose: "
    choice = Integer(gets)
  end while choice >= effects.size || choice < 0
  e = effects[choice]
else
  e = effects[rand(effects.size)]
end

exec "./particled -d #{tmw_data_dir} #{effect_subdir}/#{e}"
