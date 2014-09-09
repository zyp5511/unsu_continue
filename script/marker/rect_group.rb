require_relative 'rect'

class RectGroup
	attr_accessor :rects
	attr_accessor :inferred_rects
	attr_accessor :matched
	attr_accessor :aggregated_rect
	attr_accessor :bodyX, :bodyY, 
		:bodyH,:bodyW, :bodyS
	def initialize arect=nil, airect=nil
		matched = false
		if arect!=nil
			@rects=[arect]
			@inferred_rects=[airect]
		else
			@rects=[]
			@inferred_rects=[]
		end
	end
	def infer_part(net, partID)
		cands = []
		@rects.each do |r|
			rule = net.query r.type, partID
			if rule !=nil
				cand=(rule.transform_with_type r)
				cands<<cand;
			end
		end
		res = cands.inject(:+)/cands.count
	end
	def infer_part_globally(net, dx, dy, ds)


	end
	def infer_global
		@aggregated_rect = Rect.new(-1,0,medx,medy,medw,medh)

	end
	def include arect
		if @rects.count >0
			return @rects.inject(false){|res,rec|res || (rec.include arect)}
		else 
			return false
		end
	end
	def inferred_include arect
		if @inferred_rects.count>0
			return @inferred_rects.inject(false){|res,rec|res || (rec.include arect)}
		else 
			return false
		end
	end

	def add_rect_with_inferred ar, air
		@rects << ar;
		@inferred_rects << air;
	end

	def add_rect ar
		@rects << ar;
	end

	def reset_infer table
		@inferred_rects = []
		@rects.each{|r| ir = table.transform r;@inferred_rects<<ir}
	end
	def aggregate_avg
		medx = inferred_rects.inject(0){|s,r|s+r.x}/inferred_rects.count.to_f
		medy = inferred_rects.inject(0){|s,r|s+r.y}/inferred_rects.count.to_f
		medw = inferred_rects.inject(0){|s,r|s+r.w}/inferred_rects.count.to_f
		medh = inferred_rects.inject(0){|s,r|s+r.h}/inferred_rects.count.to_f
		@aggregated_rect = Rect.new(-1,0,medx,medy,medw,medh)
	end
	def aggregate
		ax = inferred_rects.map{|r|r.x}.sort!
		ay = inferred_rects.map{|r|r.y}.sort!
		aw = inferred_rects.map{|r|r.w}.sort!  
		ah = inferred_rects.map{|r|r.h}.sort!  
		med = ->(rules){rules[rules.size/2]}
		medx = med.call(ax)
		medy = med.call(ay)
		medw = med.call(aw)
		medh = med.call(ah)
		@aggregated_rect = Rect.new(-1,0,medx,medy,medw,medh)
	end
	def aggregate_with_table table
		if @rects.count > 1 
			itc = 0;
			loop do 
				#puts "===================="
				#puts "iteration #{itc}"
				sum_delta=0
				@rects.each_with_index{|x,i| sum_delta += adjust(i,table)}
				itc+=1
				if sum_delta<0.03||itc>5
					#puts "converged"
					break
				end
			end

		else 
			return nil
		end
	end
	def adjust i,table
		#puts "*******"
		#puts "processing nodes #{i}"
		node = @rects[i]
		rest = @rects - [node];
		#puts "originally #{@rects.count}, after removing self, #{rest.count}"
		goodpairs= rest.map{|x| [(table.query x.type,node.type),x]}.reject{|x|x[0] == nil}
		if goodpairs.count>0
			sumweight=goodpairs.map{|x|x[0].r*x[0].r}.inject(:+)
			#puts "originally #{rest.count}, after removing bad ones, #{goodpairs.count}"
			adjr =goodpairs.map{|x| (x[0].transform_with_type x[1])*(x[0].r*x[0].r)}.inject{|s,x|s+=x}/sumweight
			difr = adjr - node;
			makeupr = difr/2;
			newadj = node + makeupr
			#puts "#{adjr}\t-\t#{node}\t=\t#{difr}"
			#puts "#{node}\t+\t#{makeupr}\t=\t#{newadj}\tdiff:\t#{newadj.diff node}"
			@rects[i]=newadj
		end
		0.01
	end
end
