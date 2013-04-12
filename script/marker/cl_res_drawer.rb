require_relative 'record'
require 'set'

type = ARGV[0]
oper = ARGV[1]
src = ARGV[2]
des = ARGV[3]
report = ARGV[4]

records = Record::seperate_records(src,des,IO.foreach(report))
puts "there are #{records.count} records"

if type == "if"
	clufn= ARGV[5]
	head = IO.readlines(clufn).map{|x|x.to_i}.to_set
	c = 0;
	records.each do|x|
		goodset = x.rects.select{|r|head.include? r.type }
		if goodset.count > 0
			c+=1
			if oper == "draw"
				goodset.each{|r|x.draw_rect r}
				x.export
			elsif oper == "crop"
				goodset.each{|r|x.crop_rect r}
			end
		end
	end
	puts " #{c} images are selected"
else
	records.each do|x|
		x.rects.each{|r|x.draw_rect r; x.export}
	end
end
