#!/usr/bin/env ruby
#--
# This file is part of Sonic Pi: http://sonic-pi.net
# Full project source: https://github.com/samaaron/sonic-pi
# License: https://github.com/samaaron/sonic-pi/blob/master/LICENSE.md
#
# Copyright 2013, 2014 by Sam Aaron (http://sam.aaron.name).
# All rights reserved.
#
# Permission is granted for use, copying, modification, distribution,
# and distribution of modified versions of this work as long as this
# notice is included.
#++

require_relative "../core.rb"
require_relative "../sonicpi/lib/sonicpi/synthinfo"
require_relative "../sonicpi/lib/sonicpi/util"

include SonicPi::Util


File.open( "#{cheatsheets_path}/synths.md", 'w' ) do |f|
 f << SonicPi::SynthInfo.synth_doc_markdown
end

File.open( "#{cheatsheets_path}/fx.md", 'w') do |f|
 f << SonicPi::SynthInfo.fx_doc_markdown
end

File.open( "#{cheatsheets_path}/samples.md", 'w') do |f|
 f << SonicPi::SynthInfo.samples_doc_markdown
end
