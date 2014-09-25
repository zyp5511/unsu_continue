require 'fileutils'
require 'set'

require_relative 'rect_group'
require_relative 'transform'

class VoroTable
	#pick best rules from each outier nodes to core nodes
	attr_accessor :table

	def initialize anarr
		@table = anarr
	end

	def group_quality(group)
		group.rects.map{|r|query_raw(group.abs2gbl r)}.to_set.count
	end


	def query_raw gcor
		gx,gy,gs=gcor
		maxd2 = 0;
		maxi = -1;
		@table.each_with_index do |c,i|
			d2 = (c[0]-gx)*(c[0]-gx)+(c[1]-gy)*(c[1]-gy)+(c[2]-gs)*(c[2]-gs)
			if d2>maxd2
				maxi = i;
				maxd2 = d2;
			end
		end
		maxi
	end

	def query from,to
		if from == to
			return LCTransform.new(from,from,0,0,1)
		end
		target = from*10000+to
		invtarget= to*10000+from
		if @good_set.count>0
			if @good_set.include?(target)||@good_set.include?(invtarget)
				return @table[target]
			else 
				return nil
			end
		else 
			return @table[target]
		end 

	end


	def self.loadTable(fname)
		trans= Hash.new
		centers = []
		IO.foreach(fname) do |line|
			eles = line.chomp.split.map(&:to_f)
			centers<<eles;
		end
		VoroTable.new(centers)
	end
end
