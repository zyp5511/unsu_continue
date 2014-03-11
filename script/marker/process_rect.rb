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
	transfn = ARGV[6]
	table = LCTransformTable.loadMap(transfn,1006) #hard coded cluster number, should be changed later
	records.each do|x|
		x.pick_good_set head
		if x.goodset.count > 0
			c+=1
			if oper == "list"
				puts x.filename;
			elsif oper == "draw"
				x.goodset.each{|r|x.draw_rect r}
				x.export
			elsif oper == "crop"
				x.goodset.each{|r|x.crop_rect r}
			elsif oper == "draw_inferred"
				x.goodset.each{|r|x.draw_rect (table.transform r)}
				x.export
			elsif oper == "crop_inferred"
				x.goodset.each{|r|x.crop_rect (table.transform r)}
			elsif oper == "draw_group_inferred"
				x.group_rects table
				if x.groups.values.to_set.count > 1
					x.groups.values.to_set.each_with_index do |g,i|
						g.inferred_rects.each{|r|x.draw_rect((r), x.colortab[i*10])}
						x.draw_rect(g.aggregate,"\#ffffff")
					end
					x.export
				end
			elsif oper == "draw_group"
				x.group_rects table
				if x.groups.values.to_set.count > 1
					x.groups.values.to_set.each_with_index do |g,i|
						g.rects.each{|r|x.draw_rect((r), x.colortab[i*10])}
						x.draw_rect(g.aggregate,"\#ffffff")
					end
					x.export
				end
			end
		end
	end
	puts " #{c} images are processed"
else
	records.each do|x|
		puts "there are #{x.rects.count} rects"
		x.rects.each{|r|x.crop_rect r}
	end
end
