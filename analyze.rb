#!/usr/bin/env ruby

files = {}
file_order = []

class TestRun
  def initialize
    @samples = []
  end

  def prepare!
    @samples.sort!
  end

  def <<(diff)
    @samples << diff
  end

  def nsamples
    @samples.length
  end

  def mean
    @samples.inject(:+) / @samples.length
  end

  def median
    len = @samples.length

    if len % 2 == 1
      @samples[len / 2]
    else
      (@samples[len / 2 - 1] + @samples[len / 2]) / 2
    end
  end

  def min
    @samples.min
  end

  def max
    @samples.max
  end
end

class ResultFile
  attr_reader :run_order
  attr_reader :run_map

  def initialize
    @run_order = []
    @run_map = {}
  end

  def add_run(slept_for, diff_nsec)
    if !@run_map[slept_for]
      @run_map[slept_for] = TestRun.new
      @run_order << slept_for
    end

    @run_map[slept_for] << diff_nsec
  end

  def each_run
    @run_order.each do |r|
      yield(r, @run_map[r])
    end
  end
end

# ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ #
  # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # 
# ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ # ~ #

line_regexp = /^ :: latency for (\d+) µsec: (\d+) s, (\d+) ns/

ARGF.each_line do |line|
  filename = if ARGF.filename == '-'
               'STDIN'
             else
               ARGF.filename
             end

  if !files[filename]
    file_order << filename
    result_file = ResultFile.new
    files[filename] = result_file
  else
    result_file = files[filename]
  end

  m = line_regexp.match(line)
  next unless m

  if m[2].to_i > 0
    warn %q[ WARNING: sample with a nonzero second latency, disregarding.]
    warn %q[          you should really check on your system configuration or]
    warn %q[          something because that's super messed up.]
    next
  end

  result_file.add_run(m[1].to_i, m[3].to_f)
end

def nsec_to_msec(ns)
  ns / 1000000.0
end

def time_pair(ns)
  [ns, nsec_to_msec(ns)]
end

file_order.each do |filename|
  res = files[filename]

  puts %Q[for #{filename}:]
  res.each_run do |slept_for, run|
    run.prepare!

    puts %q[  when sleeping for %dµs:] % slept_for
    puts %q[    samples: %d] % run.nsamples
    puts %q[       mean: %5.2fns (%5.4fms)] % time_pair(run.mean)
    puts %q[     median: %5.2fns (%5.4fms)] % time_pair(run.median)
    puts %q[        min: %5.2fns (%5.4fms)] % time_pair(run.min)
    puts %q[        max: %5.2fns (%5.4fms)] % time_pair(run.max)

    puts ''
  end

  puts ''
end
