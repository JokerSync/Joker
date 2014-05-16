#!/usr/bin/env ruby

require 'json'
require 'markaby'

js = JSON.parse(ARGF.read)

total_line_count = 0
total_covered_lines = 0

mab = Markaby::Builder.new
mab.html do
  head do
    title 'Coverage report'
    link :rel => 'stylesheet', :media =>'screen, projection', :href => 'style.css'
  end
  body do
    h1 'Code coverage'

    js['source_files'].each do |file|
      name = file['name']
      h2 name
      coverage = file['coverage']
      not_covered_lines = 0
      covered_lines = 0
      line_number = 0
      pre do
        file['source'].each_line do |line|
          span.number line_number.to_s
          if coverage[line_number]
            if coverage[line_number].to_i == 0
              span.code.bad line
              not_covered_lines += 1
            else
              span.code.good line
              covered_lines += 1
            end
          else
            span.code line
          end
          span.cl
          line_number += 1
        end
      end
      file_coverage = 0
      if covered_lines + not_covered_lines > 0
        file_coverage = 100 * covered_lines / (covered_lines + not_covered_lines)
      end
      p "Coverage for #{name} : #{file_coverage}% #{covered_lines}/#{covered_lines + not_covered_lines}"
      total_line_count += covered_lines + not_covered_lines
      total_covered_lines += covered_lines
    end
    total_coverage = 0
    if total_line_count > 0
      total_coverage = 100 * total_covered_lines / total_line_count
    end
    p "Total coverage : #{total_coverage}%"
  end
end

File.open("out.html", 'w') do |f|
  f.write mab.to_s
end

