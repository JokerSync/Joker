#!/usr/bin/env ruby

trap("SIGINT") { exit }

payload = ARGV[0]
script = ARGV[1]

files = [payload, script]
puts "Watching following files for changes:"
files.each do |file|
  puts "- " + file
end

while true do
  new_hash = files.collect {|f| [ f, File.stat(f).mtime.to_i ] }
  hash ||= new_hash
  diff_hash = new_hash - hash

  unless diff_hash.empty?
    hash = new_hash

    diff_hash.each do |df|
      puts "Detected change in #{df[0]}, refreshing"
      system "#{script} #{payload}"
    end
  end

  sleep 1
end
