class Node
	include Comparable
	attr_reader :tp, :fp
	attr_reader :name
	def initialize(atp,afp,aname)
		@tp = atp;
		@fp = afp;
		@name = aname;
	end
	def <=>(another)
		comp1 = @fp.to_f/@tp <=>another.fp.to_f/another.tp
		comp2 = -@tp<=>-another.tp
		if comp1==0
			return comp2
		else
			return comp1
		end
	end
	def to_s
		"name:#{@name}\ttp:#{@tp}\tfp:#{@fp}"
	end
end

tps = IO.readlines("./data/tphist.txt").map{|x|x.split().last.to_i}.to_a
fps = IO.readlines("./data/fphist.txt").map{|x|x.split().last.to_i}.to_a


cand = tps.zip(fps).map.with_index{|x,i|Node.new(x[0],x[1],i)}.select{|x|x.tp!=0}.sort
cand.each{|x| puts x}

#puts "there are #{cand.count} candidates"
#st = 0;
#sf = 0;
#total =3306.0
#
#cand.each do |x|
#	st +=x.tp
#	sf +=x.fp
#	puts "#{st}\t#{st/total}\t#{sf}"
#end
