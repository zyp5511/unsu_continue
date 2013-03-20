require 'set'
clufn = ARGV[0]
sepfn = ARGV[1]
csvfn = ARGV[2]
sep=IO.readlines(sepfn).map{|x|x.to_i}
clu=IO.readlines(clufn).map{|x|x.to_i}
sep<<clu.count


clusize = (1..(sep.size-1)).map{|i|sep[i]-sep[i-1]}
cluset = Set.new
clusize.each{|x| cluset<<x}
puts clusize.inspect
puts cluset.inspect

