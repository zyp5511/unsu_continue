require 'RMagick'

src = ARGV[0]
des = ARGV[1]
report = ARGV[2]


class Record
	@@colors = Hash.new{|h,k|h[k]="\##{Random.rand(16777216).to_i.to_s(16).rjust(6,'0')}"}
	def initialize(src,des,lines)
		@filename = lines[0];
		@vectors = lines[1]
		@rects = lines.drop(2)
		@dest = des
		@ori = Magick::Image.read(File.join(src,@filename).to_s).first
	end

	def draw_rects
		@rects.each{|r| self.draw_rect(r)};
		@ori.write(File.join(@dest,@filename).to_s)
	end
	def draw_rect(desc)
		
		eles = desc.split
		type = eles[0].to_i
		x,y,w,h = eles[2].split(':').map{|x|x.to_i};

		rect = Magick::Draw.new
		rect.text(x,y+10,type.to_s)
		rect.stroke(@@colors[type]).stroke_width(0.5)
		rect.fill("transparent")
		rect.rectangle(x,y,x+w-1,y+h-1)
		rect.draw(@ori)
	end

	def self.seperate_records(src,des,lines)
		lines.map{|x|x.chomp}.chunk{|l|l.end_with?("jpg")}.each_slice(2).map{|a| Record.new(src,des,a[0][1]+a[1][1])}
	end
end

records = Record::seperate_records(src,des,IO.foreach(report))
records.each{|x|x.draw_rects }

