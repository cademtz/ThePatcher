Dialog.Msgbox("Hello!", "Click OK to select the example application (ExampleBinary.exe)");

-- Gets current directory from args
-- first arg is always the current LUA file path
local path,file,ext = string.match(arg[1], "(.-)([^\\]-([^\\%.]+))$")

-- Open a file dialog defaulting to this script's folder
-- All these params are optional
print('arg: ' .. arg[1])
local file = Dialog.File(path, "Select the example application.")

local pe = PortableExecutable.create(file, 1024 * 512) -- Load file with 0.5 GB limit (in kb)
if (pe == nil) then error("Failed to open " .. file) end

print('Traversing code...')
local code = CodeTraverse.create(pe);
code:AddPage(pe:Address(), pe:Len(), TRAVERSE_PAGE_READ)
code:AddPage(pe:BaseOfCode(), pe:SizeOfCode(), TRAVERSE_PAGE_CODE_READ)
code:Traverse(pe:EntryPoint())

local data = code:FindDataStrings('Apples are great. Not Apple, though.')
if (#data == 0) then error('No string :(') end

print('Patching...')
pe:Patch(data[1].loc, "I am patched :) Hello from LUA!\0");  -- Must have C-style terminator (0) to end string correctly

print('Saving...')
pe:Save(file .. "_patched.exe")

print("Saved to " .. file .. "_patched.exe")
print("Patched successfully!")

Dialog.Msgbox("Success", "Run both example files to see the difference!\nFile at: '" .. file .. "'")