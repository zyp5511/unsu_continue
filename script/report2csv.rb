require 'set'

repfn = ARGV[0]
csvfn = ARGV[1]

File.open(csvfn,"w") do |fout|
	IO.foreach(repfn) do |line|
		fout.puts line[8..-1] if line =~ /vector/
	end
end
