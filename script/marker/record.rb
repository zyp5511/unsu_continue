require 'RMagick'
require 'FileUtils'

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

class Record
	attr_accessor :rects
	@@colors = Hash.new{|h,k|h[k]="\##{Random.rand(16777216).to_i.to_s(16).rjust(6,'0')}"}
	def initialize(src,des,lines)
		@filename = lines[0];
		@vectors = lines[1]
		@rects = lines.drop(2).take_while{|x| !x.include?("=>")}.map{|l| Rect.makeRect(l)}
		@dest = des
		@ori = Magick::Image.read(File.join(src,@filename).to_s).first
	end

	def export
		@ori.write(File.join(@dest,@filename).to_s)
	end

	def draw_rect(rect)
		rdraw= Magick::Draw.new
		rdraw.text(rect.x,rect.y+10,rect.type.to_s)
		rdraw.stroke(@@colors[rect.type]).stroke_width(0.5)
		rdraw.fill("transparent")
		rdraw.rectangle(rect.x,rect.y,rect.x+rect.w-1,rect.y+rect.h-1)
		rdraw.draw(@ori)
	end

	def crop_rect(rect)
		temp = @ori.crop(rect.x,rect.y,rect.w,rect.h,true)
		type = rect.type
		subdir = "#{@dest}/#{type}".chomp
		if !File.directory?(subdir)
			FileUtils.mkdir(subdir)
		end
		temp.write("#{File.join(subdir,File.basename(@filename, File.extname(@filename))).to_s}_#{rect.x}:#{rect.y}:#{rect.w}:#{rect.h}_#{type}.#{File.extname(@filename)}")
	end

	def self.seperate_records(src,des,lines)
		lines.map{|x|x.chomp}.chunk{|l|l.end_with?("jpg")||l.end_with?("png") }.each_slice(2).map{|a| Record.new(src,des,a[0][1]+a[1][1])}
	end
end
