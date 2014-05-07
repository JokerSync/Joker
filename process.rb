#!/usr/bin/env ruby

require 'json'

s = gets.chomp

js = JSON.parse(s)
puts JSON.pretty_generate(js)

