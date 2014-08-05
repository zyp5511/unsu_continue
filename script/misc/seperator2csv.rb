require 'set'
clufn = ARGV[0]
sepfn = ARGV[1]
csvfn = ARGV[2]
sep=IO.readlines(sepfn).map{|x|x.to_i}
clu=IO.readlines(clufn).map{|x|x.to_i}
sep<<clu.count


clusize = (1..(sep.size-1)).map{|i|sep[i]-sep[i-1]}
rt=clusize.each_with_index.flat_map{|x,i| [i]*x}
vecs = Array.new(sep.count-1){Array.new(clu.max+1,0)}
(clu.zip rt).each{|cid, iid| vecs[iid][cid]+=1}

File.open(csvfn,"w") do |fout|
	vecs.each do |img|
		fout.puts img.join(",")
	end
end
