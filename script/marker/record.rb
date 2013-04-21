require 'mini_magick'
require 'FileUtils'
require_relative 'rect'
require_relative 'transform'

class Record
	attr_accessor :rects
	@@colors = Hash.new{|h,k|h[k]="\##{Random.rand(16777216).to_i.to_s(16).rjust(6,'0')}"}
	def initialize(src,des,lines)
		@filename = lines[0];
		@vectors = lines[1]
		@rects = lines.drop(2).take_while{|x| !x.include?("=>")}.map{|l| Rect.makeRect(l)}
		@dest = des
		@ori = MiniMagick::Image.open(File.join(src,@filename).to_s)
	end

	def export
		@ori.write(File.join(@dest,@filename).to_s)
	end

	def draw_rect(rect)
		@ori.combine_options do |c|

			c.draw("text #{rect.x},#{rect.y+10} \"#{rect.type.to_s}\"")

			c.stroke(@@colors[rect.type])
			c.stroke_width(0.5)
			c.fill("transparent")
			c.draw("rectangle #{rect.x},#{rect.y},#{rect.x+rect.w-1},#{rect.y+rect.h-1}")
		end
	end

	def crop_rect(rect)
		temp = @ori.clone
		cropargs="#{rect.w}x#{rect.h}+#{rect.x}+#{rect.y}"
		puts cropargs
		temp.crop(cropargs)
		
		type = rect.type
		subdir = "#{@dest}/#{type}".chomp
		if !File.directory?(subdir)
			FileUtils.mkdir(subdir)
		end
		temp.write("#{File.join(subdir,File.basename(@filename, File.extname(@filename))).to_s}_#{rect.x}+#{rect.y}+#{rect.w}x#{rect.h}_#{type}#{File.extname(@filename)}")
	end

	def self.seperate_records(src,des,lines)
		lines.map{|x|x.chomp}.chunk{|l|l.end_with?("jpg")||l.end_with?("png") }.each_slice(2).map{|a| Record.new(src,des,a[0][1]+a[1][1])}
	end
end
