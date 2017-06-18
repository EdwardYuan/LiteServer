
local socket = require("socket")

print("IP:")
local host = io.read()
if host == "" then 
	host = "127.0.0.1"
end
print("Port:")
local port = io.read()

local client_num = 100005

local socket_list = {}
for i=1,client_num do
	local sock = socket.connect(host, port)	
	if sock then 
		socket_list[i] = sock
		sock:settimeout(0)
	end
	if i % 1000 == 0 then 
		print("connect" .. i)
	end
end

function GetPackageData(data)
	local str = ""
	local length = string.len(data) + 1
	str = str .. string.char(0,1,0,1,0,1,0,0,0,1,0,length)	
	str = str .. data
	str = str .. string.char(0)
	return str
end

-- for i=1,1 do
-- 	for sock=1,client_num do
-- 		local data = "MSG" .. sock
-- 		local str = GetPackageData(data)
-- 		local ret = socket_list[sock]:send(str)
-- 		for c=1,string.len(str) do
-- 			socket_list[sock]:send(string.sub(str, c, c))
-- 		end
-- 	end
-- end
io.read()

for i=1,client_num do
	if socket_list[i] then
		socket_list[i]:close()
	end
end

