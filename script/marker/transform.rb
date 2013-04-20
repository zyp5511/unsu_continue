
class LCTransform
	include Comparable
	attr_accessor :from, :to, :xr, :yr, :r
	def initialize(from, to, xr, yr, r)
		@from = from;
		@to = to;
		@xr = xr;
		@yr = yr;
		@r = r;
	end

	def self.load (str)
		from_str,to_str,xr_str,yr_str,r_str = str.split(/=>|:|\s/).map(&:chomp)
		LCTransform.new(from_str.to_i,to_str.to_i,xr_str.to_f,yr_str.to_f,r_str.to_f)
	end

	def to_s
		"#{@from}=>#{@to}\t#{@xr}:#{@yr}:#{@r}"
	end
	def <=>(other)# for sorting
		comparision = r <=> other.r
		if comparision == 0
			return xr <=>other.xr
		else
			return comparision
		end
	end
	def +(other)# for average calculation
		LCTransform.new(from,to,xr+other.xr,yr+other.yr,r+other.r)
	end
	def /(n)
		LCTransform.new(from,to,xr/n,yr/n,r/n)
	end
end

class LCTransformSet
	def initialize(transforms)
		@transforms = transforms
	end
	def self.loadAll(fname)
    trans = Array.new;
    IO.foreach(fname) do |line|
      trans << LCTransform.load(line)
    end
    LCTransformSet.new(trans)
  end
	def self.loadMap(fname,n)
		trans = Array.new(n);
		IO.foreach(fname) do |line|
			t = LCTransform.load(line)
			trans[t.from] = t;
		end
		LCTransformSet.new(trans)
	end
	def simplify
		@transforms.group_by{|t| t.from }.flat_map do |kf,fg|
			fg.group_by{|tt| tt.to}.select{|k,v| k==594}.map do |kt,tg| 
				tg[tg.size/2]#median for now, maybe use avg later
			end
		end
	end
end

src = ARGV[0]
tc = LCTransformSet.loadAll(src)
tc.simplify.each{|x| puts x}
