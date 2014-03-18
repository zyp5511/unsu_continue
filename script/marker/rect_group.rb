require_relative 'rect'

class RectGroup
	attr_accessor :rects
	attr_accessor :inferred_rects
	def initialize 
	end
	def initialize arect, airect
		@rects=[arect]
		@inferred_rects=[airect]
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
	def add_rect ar, air
		@rects << ar;
		@inferred_rects << air;
	end

	def reset_infer table
		@inferred_rects = []
		@rects.each{|r| ir = table.transform r;@inferred_rects<<ir}
	end
	def aggregate
		ax = inferred_rects.map{|x|x.x}.sort!
		ay = inferred_rects.map{|x|x.y}.sort!
		aw = inferred_rects.map{|x|x.w}.sort!  
		ah = inferred_rects.map{|x|x.h}.sort!  
		med = ->(rules){rules[rules.size/2]}
		medx = med.call(ax)
		medy = med.call(ay)
		medw = med.call(aw)
		medh = med.call(ah)
		Rect.new(-1,0,medx,medy,medw,medh)
	end
	def aggregate_with_table table
		if @rects.count > 1 
			itc = 0;
			loop do 
				puts "===================="
				puts "iteration #{itc}"
				sum_delta=0
				@rects.each_with_index{|x,i| sum_delta += adjust(i,table)}
				itc+=1
				if sum_delta<0.03||itc>5
					puts "converged"
					break
				end
			end

		else 
			return nil
		end
	end
	def adjust i,table
		puts "*******"
		puts "processing nodes #{i}"
		node = @rects[i]
		rest = @rects - [node];
		puts "originally #{@rects.count}, after removing self, #{rest.count}"
		goodpairs= rest.map{|x| [(table.query x.type,node.type),x]}.reject{|x|x[0] == nil}
		if goodpairs.count>0
			sumweight=goodpairs.map{|x|x[0].r*x[0].r}.inject(:+)
			puts "originally #{rest.count}, after removing bad ones, #{goodpairs.count}"
			adjr =goodpairs.map{|x| (x[0].transform_with_type x[1])*(x[0].r*x[0].r)}.inject{|s,x|s+=x}/sumweight
			difr = adjr - node;
			makeupr = difr/2;
			newadj = node + makeupr
			puts "#{adjr}\t-\t#{node}\t=\t#{difr}"
			puts "#{node}\t+\t#{makeupr}\t=\t#{newadj}"
			@rects[i]=newadj
		end
		0.01
	end

end
