#!/bin/env ruby -w

# pe_runner.rb
# igneus 10.2.2009

# simple wrapper for particlEd;
# has simple feature to choose effect
#
# Options:
# option -r chooses random effect
# anything behind pseudo-option '--' is given as argument to particled

# Configuration area =================================================
tmw_data_dir = '/home/igneus/src/cplusplus/particlEd/trunk'
effect_subdir = 'graphics/particles'
# end of configuration area ==========================================

require 'rubygems'
require 'trollop'

# commandline options:
opts = Trollop::options do
  opt :random, "Choose random effect"
  stop_on ['--']
end

unless ARGV.empty?
  ARGV.shift # remove '--'
  options_for_particled = ARGV.join ' '
end

effects = Dir[tmw_data_dir+'/'+effect_subdir+'/*.particle.xml'].sort.collect do |f|
  File.basename f
end

if opts[:random] then
  e = effects[rand(effects.size)]
  puts "Random effect: '#{e}'"
else
  effects.each_with_index {|f,i| puts "#{i.to_s.rjust(3)}| #{f}"}
  choice = nil
  begin
    print "choose: "
    choice = Integer(STDIN.gets)
  end while choice >= effects.size || choice < 0
  e = effects[choice]
end

cmd = "./particled #{options_for_particled} -d #{tmw_data_dir} #{effect_subdir}/#{e}"
puts cmd
exec cmd
