src = ARGV[0]
des = ARGV[1]

class Record
	@img;
	def draw_record(lines)
		lines = lines.collect
		fn = lines[1];
		vectors = lines[2]
		rects = lines.drop(2)
	end

	def draw_rect(desc)

	end

	def self.seperate_records(lines)
		lines.chunk{|l|l=~/jpg/}.each_slice(2){|a| yeild (a[0][1]+a[1][1])}
	end
end
