class Rect
	attr_accessor :type, :dis, :x,:y,:w,:h
	def initialize(type,dis,x,y,w,h)
		@type = type
		@dis = dis
		@x = x
		@y = y
		@w = w
		@h = h
	end
	def self.makeRect(desc)
		eles = desc.split
		type = eles[0].to_i
		dis = eles[1].to_f;
		x,y,w,h = eles[2].split(':').map{|x|x.to_i};
		Rect.new(type,dis,x,y,w,h)
	end
	
end
