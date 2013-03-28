require 'RMagick'

src = ARGV[0]
des = ARGV[1]
report = ARGV[2]


class Record
	def initialize(src,des,lines)
		@filename = lines[0];
		@vectors = lines[1]
		@rects = lines.drop(2)
		@dest = des
		@ori = Magick::Image.read(File.join(src,@filename).to_s).first
	end

	def draw_rects
		#@rects.each{|r| self.draw_rect(r)};
		self.draw_rect(@rects[2])
	end
	def draw_rect(desc)
		
		eles = desc.split
		type = eles[0]
		x,y,w,h = eles[2].split(':').map{|x|x.to_i};
		temp = @ori.clone

		rect = Magick::Draw.new
		rect.stroke('black').stroke_width(1)
		rect.text(x,y,type.to_s)
		rect.fill("transparent")
		rect.rectangle(x,y,x+w-1,y+h-1)
		rect.draw(temp)
		temp.write(File.join(@dest,@filename).to_s)
	end

	def self.seperate_records(src,des,lines)
		lines.map{|x|x.chomp}.chunk{|l|l.end_with?("jpg")}.each_slice(2).map{|a| Record.new(src,des,a[0][1]+a[1][1])}
	end
end

records = Record::seperate_records(src,des,IO.foreach(report))
records.each{|x|x.draw_rects }

