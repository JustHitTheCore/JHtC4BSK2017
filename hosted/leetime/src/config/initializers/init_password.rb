def let(l)
  l * 1337.1337
end
  
original_flag = "JHtC4BSK{Dat_71M1NG_attack}"

flag = ""
time = Benchmark.ms {
  original_flag.split("").each do |l|
    flag += let(l)
  end
}

ENV['PASS'] = flag
puts "Done in: #{time}"
puts "Length: #{original_flag.length}"

