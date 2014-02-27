class Rect
	attr_accessor :type, :dis, :x,:y,:w,:h,:matched
	def initialize(type,dis,x,y,w,h)
		@type = type
		@dis = dis
		@x = x
		@y = y
		@w = w
		@h = h
		@matched = false
	end
	def self.makeRect(desc)
		eles = desc.split
		type = eles[0].to_i
		dis = eles[1].to_f;
		x,y,w,h = eles[2].split(':').map(&:to_i)
		Rect.new(type,dis,x,y,w,h)
	end
	def self.makePureRect(sdesc)
		x,y,w,h=sdesc.split(':').map(&:to_i)
		Rect.new(-1,0,x,y,w,h)
	end

	def to_s
		"#{@type}:#{@x}:#{@y}:#{@w}:#{@h}"
	end
	
	def to_short_s
		"#{@x}:#{@y}:#{@w}:#{@h}"
	end

	def has_point x,y
		dx = x - @x;
		dy = y - @y;
		if(dx>0&&w>dx&&dy>0&&h>dy)
			return true;
		else
			return false;
		end
	end
	
end
