print("TEST!")

function NewFile(name, context)
	local file = io.open(name, 'w')
	if file then
		file:write(context)
		file:close()
	end
end

NewFile('test.txt', "TEXT")
