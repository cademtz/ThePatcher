#pragma once
#include <list>
#include <stdint.h>
#include <codetraverse/traverse.h>
#include <codetraverse/portable_executable.h>

typedef struct lua_State lua_State;
class CLuaManagedPtrBase;
struct FuncPair;

typedef struct _traverse_func TraverseFunc;

class CLuaPatcher
{
public:
	CLuaPatcher(struct lua_State* L);
	~CLuaPatcher();

	static int _pe_new(lua_State* L);
	static int _pe_addr(lua_State* L);
	static int _pe_len(lua_State* L);
	static int _pe_base(lua_State* L);
	static int _pe_codeaddr(lua_State* L);
	static int _pe_codelen(lua_State* L);
	static int _pe_entry(lua_State* L);
	static int _pe_nexps(lua_State* L);
	static int _pe_exp(lua_State* L);
	static int _pe_nrtns(lua_State* L);
	static int _pe_rtn(lua_State* L);
	static int _pe_patch(lua_State* L);
	static int _pe_patchhex(lua_State* L);
	static int _pe_save(lua_State* L);

	static int _ct_new(lua_State* L);
	static int _ct_ndatas(lua_State* L);
	static int _ct_data(lua_State* L);
	static int _ct_nfuncs(lua_State* L);
	static int _ct_func(lua_State* L);
	static int _ct_findfunc(lua_State* L);
	static int _ct_strref(lua_State* L);
	static int _ct_addpage(lua_State* L);
	static int _ct_traverse(lua_State* L);
	static int _ct_readstr(lua_State* L);

	static int _dlg_file(lua_State* L);
	static int _dlg_msgbox(lua_State* L);

private:
	/**@brief	Ensures pointer is deleted when owning LuaPatcher instance closes */
	template <typename T>
	static inline void PushNewInstance(lua_State* L, T* Inst);

	static void PushInstance(lua_State* L, CLuaManagedPtrBase* Ptr);
	static void PushTraverseFunc(lua_State* L, const TraverseFunc* Func);
	static void PushTraverseData(lua_State* L, const TraverseData* Data);
	static void PushAddress(lua_State* L, uint64_t Addr);

	template <class T>
	static inline void PushAddress(lua_State* L, T Addr) {
		return PushAddress(L, (uint64_t)Addr);
	}

	struct lua_State* m_state;
	std::list<CLuaManagedPtrBase*> m_managed;
};

class CLuaManagedPtrBase {
public:
	virtual ~CLuaManagedPtrBase() = 0 { }
	virtual void* _Inst() = 0;
};

/**@brief	Helps with correct deletion of types on cleanup */
template <class T>
class CLuaManagedPtr : public CLuaManagedPtrBase
{
public:
	CLuaManagedPtr(T* Inst) : m_inst(Inst) { }
	~CLuaManagedPtr() { delete m_inst; }

	void* _Inst() override { return m_inst; }
	inline T* Inst() { return m_inst; }

private:
	T* m_inst;
};

// ---------- Functions ----------

template<typename T>
inline void CLuaPatcher::PushNewInstance(lua_State* L, T* Inst) {
	PushInstance(L, new CLuaManagedPtr<T>(Inst));
}
