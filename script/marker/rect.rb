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

	def include other
		c1 = self.has_point other.x+(other.w/2),other.y+(other.h/2)
		c2 = other.has_point self.x+(self.w/2),self.y+(self.h/2)
		c1 || c2
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

	def *(k)# for average calculation
		atype = @type
		adis = @dis;
		ax = (@x*k).to_i
		ay = (@y*k).to_i
		aw = (@w*k).to_i
		ah = (@h*k).to_i
		Rect.new(atype,adis,ax,ay,aw,ah)
	end

	def +(other)# for average calculation
		atype = @type
		adis = 0;
		ax = @x +other.x
		ay = @y +other.y
		aw = @w +other.w
		ah = @h +other.h
		Rect.new(atype,adis,ax,ay,aw,ah)
	end

	def -(other)# for difference calculation
		atype = @type
		adis = 0;
		ax = @x -other.x
		ay = @y -other.y
		aw = @w -other.w
		ah = @h -other.h
		Rect.new(atype,adis,ax,ay,aw,ah)
	end

	def /(n)
		Rect.new(@type,@dis,(@x.to_f/n).to_i,(@y.to_f/n).to_i,
						 (@w.to_f/n).to_i,(@h.to_f/n).to_i)
	end
end

