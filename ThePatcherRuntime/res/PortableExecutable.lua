
-- Thoughtfully copied here for your reference.
-- Editing this file will not change the behavior. It is actually loaded from an embedded resource file.

-- Notes:
--	Arrays typically start at 1, in LUA
--	FindDataStrings() finds strings with code refs only. This will not string search the entire file

TRAVERSE_PAGE_NONE = 0
TRAVERSE_PAGE_CODE = 1
TRAVERSE_PAGE_READ = 2
TRAVERSE_PAGE_WRITE = 4

TRAVERSE_PAGE_READWRITE =		TRAVERSE_PAGE_READ + TRAVERSE_PAGE_WRITE
TRAVERSE_PAGE_CODE_READ =		TRAVERSE_PAGE_CODE + TRAVERSE_PAGE_READ
TRAVERSE_PAGE_CODE_WRITE =		TRAVERSE_PAGE_CODE + TRAVERSE_PAGE_WRITE
TRAVERSE_PAGE_CODE_READWRITE =	TRAVERSE_PAGE_CODE + TRAVERSE_PAGE_READWRITE

PortableExecutable = {}
CodeTraverse = {}
Dialog = {}

PortableExecutable.__index = PortableExecutable
CodeTraverse.__index = CodeTraverse

function PortableExecutable.create(file, size)
	local object = {}
	object._inst = pe_api._new(file, size)

	return setmetatable(object, PortableExecutable)
end

function CodeTraverse.create(pe)
	local object = {}
	object._inst = ct_api._new(pe._inst)

	return setmetatable(object, CodeTraverse)
end

function PortableExecutable:Address()				return pe_api._addr(self._inst)						end
function PortableExecutable:Len()					return pe_api._len(self._inst)						end
function PortableExecutable:SizeOfCode()			return pe_api._codelen(self._inst)					end
function PortableExecutable:BaseOfCode()			return pe_api._codeaddr(self._inst)					end
function PortableExecutable:EntryPoint()			return pe_api._entry(self._inst)					end
function PortableExecutable:NumExports()			return pe_api._nexps(self._inst)					end
function PortableExecutable:Export(index)			return pe_api._exp(self._inst, index)				end
function PortableExecutable:Patch(addr, bytes)		return pe_api._patch(self._inst, addr, bytes)		end
function PortableExecutable:PatchHex(addr, bytes)	return pe_api._patchhex(self._inst, addr, bytes)	end
function PortableExecutable:Save(fname)				return pe_api._save(self._inst, fname)				end

function CodeTraverse:NumData()					return ct_api._ndatas(self._inst)						end
function CodeTraverse:Data(index)				return ct_api._data(self._inst, index)					end
function CodeTraverse:NumFuncs()				return ct_api._nfuncs(self._inst)						end
function CodeTraverse:Func(index)				return ct_api._func(self._inst, index)					end
function CodeTraverse:FindFunc(addr)			return ct_api._findfunc(self._inst, addr)				end
function CodeTraverse:FindDataStrings(str)		return ct_api._strref(self._inst, str)					end
function CodeTraverse:Traverse(addr)			return ct_api._traverse(self._inst, addr)				end
function CodeTraverse:AddPage(addr, len, flags)	return ct_api._addpage(self._inst, addr, len, flags)	end
function CodeTraverse:ReadString(addr, maxlen)	return ct_api._readstr(self._inst, addr, maxlen)		end

-- All args are optional
function Dialog.File(path, title)		return dialog_api._file(path, title)		end
function Dialog.Msgbox(title, content)	return dialog_api._msgbox(title, content)	end