function println( thing )
	io.write(thing .. "\n")
end

println("When entering lua, song length = " .. getLength())

setLength(8)
println("When leaving lua, song length = " .. getLength())