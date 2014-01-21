require_relative 'rect'

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
	def self.extract ri, rj
		ii = ri.type
		jj = rj.type
		xx = (rj.x - ri.x+0.0)/ri.w
		yy = (rj.y - ri.y+0.0)/ri.h
		rr = (rj.h + 0.0)/ri.h
		LCTransform.new(ii,jj,xx,yy,rr)
	end
	def transform rect 
		x = rect.x + xr * rect.w
		y = rect.y + yr * rect.h
		w = rect.w * r
		h = rect.h * r
		Rect.new(rect.type,rect.dis,x.to_i,y.to_i,w.to_i,h.to_i)
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
		@simplify_approaches ={
			median: ->(rules){rules[rules.size/2]},
			avg:->(rules)do
				if rules.size<3
					return rules[0]
				else 
					rules[1...-1].inject(:+)/(rules.size-2)
				end
			end
		}
	end
	def self.loadAll(fname)
		trans = Array.new
		IO.foreach(fname) do |line|
			trans << LCTransform.load(line) if line =~ /=>/
		end
		LCTransformSet.new(trans)
	end
	def simplify (appr,id=594)# rule extraction using different lambda
		@transforms.group_by{|t| t.from }.flat_map do |kf,fg|
			fg.group_by{|tt| tt.to}.select{|k,v| k==id}.map do |kt,tg| 
				sorted_tg = tg.sort
				@simplify_approaches[appr].call(sorted_tg)
			end
		end
	end
end

class LCTransformTable < LCTransformSet
	def transform rect
		t = @transforms[rect.type]
		if t!=nil
			t.transform rect
		else
			rect
		end
	end	
	def self.loadMap(fname,n)
		trans = Array.new(n);
		IO.foreach(fname) do |line|
			t = LCTransform.load(line)
			trans[t.from] = t;
		end
		LCTransformTable.new(trans)
	end
end


