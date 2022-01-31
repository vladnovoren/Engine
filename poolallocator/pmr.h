
#include <limits>			// numeric_limits
#include <memory>			// align, allocator_arg_t, __uses_alloc
#include <utility>			// pair, index_sequence
#include <vector>			// vector
#include <cstddef>			// size_t, max_align_t, byte
#include <shared_mutex>			// shared_mutex
#include <bits/functexcept.h>
#include <debug/assertions.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
namespace pmr
{
#ifdef _GLIBCXX_HAS_GTHREADS
  // Header and all contents are present.
# define __cpp_lib_memory_resource 201603
#else
  // The pmr::synchronized_pool_resource type is missing.
# define __cpp_lib_memory_resource 1
#endif

  class memory_resource;

#if __cplusplus == 201703L
  template<typename _Tp>
    class polymorphic_allocator;
#else // C++20
  template<typename _Tp = std::byte>
    class polymorphic_allocator;
#endif

  // Global memory resources
  memory_resource* new_delete_resource() noexcept;
  memory_resource* null_memory_resource() noexcept;
  memory_resource* set_default_resource(memory_resource* __r) noexcept;
  memory_resource* get_default_resource() noexcept
    __attribute__((__returns_nonnull__));

  // Pool resource classes
  struct pool_options;
#ifdef _GLIBCXX_HAS_GTHREADS
  class synchronized_pool_resource;
#endif
  class unsynchronized_pool_resource;
  class monotonic_buffer_resource;

  /// Class memory_resource
  class memory_resource
  {
    static constexpr size_t _S_max_align = alignof(max_align_t);

  public:
    memory_resource() = default;
    memory_resource(const memory_resource&) = default;
    virtual ~memory_resource() = default;

    memory_resource& operator=(const memory_resource&) = default;

    [[nodiscard]]
    void*
    allocate(size_t __bytes, size_t __alignment = _S_max_align)
    __attribute__((__returns_nonnull__,__alloc_size__(2),__alloc_align__(3)))
    { return do_allocate(__bytes, __alignment); }

    void
    deallocate(void* __p, size_t __bytes, size_t __alignment = _S_max_align)
    __attribute__((__nonnull__))
    { return do_deallocate(__p, __bytes, __alignment); }

    bool
    is_equal(const memory_resource& __other) const noexcept
    { return do_is_equal(__other); }

  private:
    virtual void*
    do_allocate(size_t __bytes, size_t __alignment) = 0;

    virtual void
    do_deallocate(void* __p, size_t __bytes, size_t __alignment) = 0;

    virtual bool
    do_is_equal(const memory_resource& __other) const noexcept = 0;
  };

  inline bool
  operator==(const memory_resource& __a, const memory_resource& __b) noexcept
  { return &__a == &__b || __a.is_equal(__b); }

  inline bool
  operator!=(const memory_resource& __a, const memory_resource& __b) noexcept
  { return !(__a == __b); }


  // C++17 23.12.3 Class template polymorphic_allocator
  template<typename _Tp>
    class polymorphic_allocator
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 2975. Missing case for pair construction in polymorphic allocators
      template<typename _Up>
	struct __not_pair { using type = void; };

      template<typename _Up1, typename _Up2>
	struct __not_pair<pair<_Up1, _Up2>> { };

    public:
      using value_type = _Tp;

      polymorphic_allocator() noexcept
      : _M_resource(get_default_resource())
      { }

      polymorphic_allocator(memory_resource* __r) noexcept
      __attribute__((__nonnull__))
      : _M_resource(__r)
      { _GLIBCXX_DEBUG_ASSERT(__r); }

      polymorphic_allocator(const polymorphic_allocator& __other) = default;

      template<typename _Up>
	polymorphic_allocator(const polymorphic_allocator<_Up>& __x) noexcept
	: _M_resource(__x.resource())
	{ }

      polymorphic_allocator&
      operator=(const polymorphic_allocator&) = delete;

      [[nodiscard]]
      _Tp*
      allocate(size_t __n)
      __attribute__((__returns_nonnull__))
      {
	if (__n > (numeric_limits<size_t>::max() / sizeof(_Tp)))
	  std::__throw_bad_alloc();
	return static_cast<_Tp*>(_M_resource->allocate(__n * sizeof(_Tp),
						       alignof(_Tp)));
      }

      void
      deallocate(_Tp* __p, size_t __n) noexcept
      __attribute__((__nonnull__))
      { _M_resource->deallocate(__p, __n * sizeof(_Tp), alignof(_Tp)); }

#if __cplusplus > 201703L
      void*
      allocate_bytes(size_t __nbytes,
		     size_t __alignment = alignof(max_align_t))
      { return _M_resource->allocate(__nbytes, __alignment); }

      void
      deallocate_bytes(void* __p, size_t __nbytes,
		       size_t __alignment = alignof(max_align_t))
      { _M_resource->deallocate(__p, __nbytes, __alignment); }

      template<typename _Up>
	_Up*
	allocate_object(size_t __n = 1)
	{
	  if ((std::numeric_limits<size_t>::max() / sizeof(_Up)) < __n)
	    __throw_length_error("polymorphic_allocator::allocate_object");
	  return static_cast<_Up*>(allocate_bytes(__n * sizeof(_Up),
						  alignof(_Up)));
	}

      template<typename _Up>
	void
	deallocate_object(_Up* __p, size_t __n = 1)
	{ deallocate_bytes(__p, __n * sizeof(_Up), alignof(_Up)); }

      template<typename _Up, typename... _CtorArgs>
	_Up*
	new_object(_CtorArgs&&... __ctor_args)
	{
	  _Up* __p = allocate_object<_Up>();
	  __try
	    {
	      construct(__p, std::forward<_CtorArgs>(__ctor_args)...);
	    }
	  __catch (...)
	    {
	      deallocate_object(__p);
	      __throw_exception_again;
	    }
	  return __p;
	}

      template<typename _Up>
	void
	delete_object(_Up* __p)
	{
	  destroy(__p);
	  deallocate_object(__p);
	}
#endif // C++2a

#if __cplusplus == 201703L
      template<typename _Tp1, typename... _Args>
	__attribute__((__nonnull__))
	typename __not_pair<_Tp1>::type
	construct(_Tp1* __p, _Args&&... __args)
	{
	  // _GLIBCXX_RESOLVE_LIB_DEFECTS
	  // 2969. polymorphic_allocator::construct() shouldn't pass resource()
	  using __use_tag
	    = std::__uses_alloc_t<_Tp1, polymorphic_allocator, _Args...>;
	  if constexpr (is_base_of_v<__uses_alloc0, __use_tag>)
	    ::new(__p) _Tp1(std::forward<_Args>(__args)...);
	  else if constexpr (is_base_of_v<__uses_alloc1_, __use_tag>)
	    ::new(__p) _Tp1(allocator_arg, *this,
			    std::forward<_Args>(__args)...);
	  else
	    ::new(__p) _Tp1(std::forward<_Args>(__args)..., *this);
	}

      template<typename _Tp1, typename _Tp2,
	       typename... _Args1, typename... _Args2>
	__attribute__((__nonnull__))
	void
	construct(pair<_Tp1, _Tp2>* __p, piecewise_construct_t,
		  tuple<_Args1...> __x, tuple<_Args2...> __y)
	{
	  auto __x_tag =
	    __use_alloc<_Tp1, polymorphic_allocator, _Args1...>(*this);
	  auto __y_tag =
	    __use_alloc<_Tp2, polymorphic_allocator, _Args2...>(*this);
	  index_sequence_for<_Args1...> __x_i;
	  index_sequence_for<_Args2...> __y_i;

	  ::new(__p) pair<_Tp1, _Tp2>(piecewise_construct,
				      _S_construct_p(__x_tag, __x_i, __x),
				      _S_construct_p(__y_tag, __y_i, __y));
	}

      template<typename _Tp1, typename _Tp2>
	__attribute__((__nonnull__))
	void
	construct(pair<_Tp1, _Tp2>* __p)
	{ this->construct(__p, piecewise_construct, tuple<>(), tuple<>()); }

      template<typename _Tp1, typename _Tp2, typename _Up, typename _Vp>
	__attribute__((__nonnull__))
	void
	construct(pair<_Tp1, _Tp2>* __p, _Up&& __x, _Vp&& __y)
	{
	  this->construct(__p, piecewise_construct,
			  forward_as_tuple(std::forward<_Up>(__x)),
			  forward_as_tuple(std::forward<_Vp>(__y)));
	}

      template <typename _Tp1, typename _Tp2, typename _Up, typename _Vp>
	__attribute__((__nonnull__))
	void
	construct(pair<_Tp1, _Tp2>* __p, const std::pair<_Up, _Vp>& __pr)
	{
	  this->construct(__p, piecewise_construct,
			  forward_as_tuple(__pr.first),
			  forward_as_tuple(__pr.second));
	}

      template<typename _Tp1, typename _Tp2, typename _Up, typename _Vp>
	__attribute__((__nonnull__))
	void
	construct(pair<_Tp1, _Tp2>* __p, pair<_Up, _Vp>&& __pr)
	{
	  this->construct(__p, piecewise_construct,
			  forward_as_tuple(std::forward<_Up>(__pr.first)),
			  forward_as_tuple(std::forward<_Vp>(__pr.second)));
	}
#else
      template<typename _Tp1, typename... _Args>
	__attribute__((__nonnull__))
	void
	construct(_Tp1* __p, _Args&&... __args)
	{
	  std::uninitialized_construct_using_allocator(__p, *this,
	      std::forward<_Args>(__args)...);
	}
#endif

      template<typename _Up>
	__attribute__((__nonnull__))
	void
	destroy(_Up* __p)
	{ __p->~_Up(); }

      polymorphic_allocator
      select_on_container_copy_construction() const noexcept
      { return polymorphic_allocator(); }

      memory_resource*
      resource() const noexcept
      __attribute__((__returns_nonnull__))
      { return _M_resource; }

    private:
      using __uses_alloc1_ = __uses_alloc1<polymorphic_allocator>;
      using __uses_alloc2_ = __uses_alloc2<polymorphic_allocator>;

      template<typename _Ind, typename... _Args>
	static tuple<_Args&&...>
	_S_construct_p(__uses_alloc0, _Ind, tuple<_Args...>& __t)
	{ return std::move(__t); }

      template<size_t... _Ind, typename... _Args>
	static tuple<allocator_arg_t, polymorphic_allocator, _Args&&...>
	_S_construct_p(__uses_alloc1_ __ua, index_sequence<_Ind...>,
		       tuple<_Args...>& __t)
	{
	  return {
	      allocator_arg, *__ua._M_a, std::get<_Ind>(std::move(__t))...
	  };
	}

      template<size_t... _Ind, typename... _Args>
	static tuple<_Args&&..., polymorphic_allocator>
	_S_construct_p(__uses_alloc2_ __ua, index_sequence<_Ind...>,
		       tuple<_Args...>& __t)
	{ return { std::get<_Ind>(std::move(__t))..., *__ua._M_a }; }

      memory_resource* _M_resource;
    };

  template<typename _Tp1, typename _Tp2>
    inline bool
    operator==(const polymorphic_allocator<_Tp1>& __a,
	       const polymorphic_allocator<_Tp2>& __b) noexcept
    { return *__a.resource() == *__b.resource(); }

  template<typename _Tp1, typename _Tp2>
    inline bool
    operator!=(const polymorphic_allocator<_Tp1>& __a,
	       const polymorphic_allocator<_Tp2>& __b) noexcept
    { return !(__a == __b); }


  /// Parameters for tuning a pool resource's behaviour.
  struct pool_options
  {
    /** @brief Upper limit on number of blocks in a chunk.
     *
     * A lower value prevents allocating huge chunks that could remain mostly
     * unused, but means pools will need to replenished more frequently.
     */
    size_t max_blocks_per_chunk = 0;

    /* @brief Largest block size (in bytes) that should be served from pools.
     *
     * Larger allocations will be served directly by the upstream resource,
     * not from one of the pools managed by the pool resource.
     */
    size_t largest_required_pool_block = 0;
  };

  // Common implementation details for un-/synchronized pool resources.
  class __pool_resource
  {
    friend class synchronized_pool_resource;
    friend class unsynchronized_pool_resource;

    __pool_resource(const pool_options& __opts, memory_resource* __upstream);

    ~__pool_resource();

    __pool_resource(const __pool_resource&) = delete;
    __pool_resource& operator=(const __pool_resource&) = delete;

    // Allocate a large unpooled block.
    void*
    allocate(size_t __bytes, size_t __alignment);

    // Deallocate a large unpooled block.
    void
    deallocate(void* __p, size_t __bytes, size_t __alignment);


    // Deallocate unpooled memory.
    void release() noexcept;

    memory_resource* resource() const noexcept
    { return _M_unpooled.get_allocator().resource(); }

    struct _Pool;

    _Pool* _M_alloc_pools();

    const pool_options _M_opts;

    struct _BigBlock;
    // Collection of blocks too big for any pool, sorted by address.
    // This also stores the only copy of the upstream memory resource pointer.
    _GLIBCXX_STD_C::pmr::vector<_BigBlock> _M_unpooled;

    const int _M_npools;
  };

#ifdef _GLIBCXX_HAS_GTHREADS
  /// A thread-safe memory resource that manages pools of fixed-size blocks.
  class synchronized_pool_resource : public memory_resource
  {
  public:
    synchronized_pool_resource(const pool_options& __opts,
				 memory_resource* __upstream)
    __attribute__((__nonnull__));

    synchronized_pool_resource()
    : synchronized_pool_resource(pool_options(), get_default_resource())
    { }

    explicit
    synchronized_pool_resource(memory_resource* __upstream)
    __attribute__((__nonnull__))
    : synchronized_pool_resource(pool_options(), __upstream)
    { }

    explicit
    synchronized_pool_resource(const pool_options& __opts)
    : synchronized_pool_resource(__opts, get_default_resource()) { }

    synchronized_pool_resource(const synchronized_pool_resource&) = delete;

    virtual ~synchronized_pool_resource();

    synchronized_pool_resource&
    operator=(const synchronized_pool_resource&) = delete;

    void release();

    memory_resource*
    upstream_resource() const noexcept
    __attribute__((__returns_nonnull__))
    { return _M_impl.resource(); }

    pool_options options() const noexcept { return _M_impl._M_opts; }

  protected:
    void*
    do_allocate(size_t __bytes, size_t __alignment) override;

    void
    do_deallocate(void* __p, size_t __bytes, size_t __alignment) override;

    bool
    do_is_equal(const memory_resource& __other) const noexcept override
    { return this == &__other; }

  public:
    // Thread-specific pools (only public for access by implementation details)
    struct _TPools;

  private:
    _TPools* _M_alloc_tpools(lock_guard<shared_mutex>&);
    _TPools* _M_alloc_shared_tpools(lock_guard<shared_mutex>&);
    auto _M_thread_specific_pools() noexcept;

    __pool_resource _M_impl;
    __gthread_key_t _M_key;
    // Linked list of thread-specific pools. All threads share _M_tpools[0].
    _TPools* _M_tpools = nullptr;
    mutable shared_mutex _M_mx;
  };
#endif

  /// A non-thread-safe memory resource that manages pools of fixed-size blocks.
  class unsynchronized_pool_resource : public memory_resource
  {
  public:
    [[__gnu__::__nonnull__]]
    unsynchronized_pool_resource(const pool_options& __opts,
				 memory_resource* __upstream);

    unsynchronized_pool_resource()
    : unsynchronized_pool_resource(pool_options(), get_default_resource())
    { }

    [[__gnu__::__nonnull__]]
    explicit
    unsynchronized_pool_resource(memory_resource* __upstream)
    : unsynchronized_pool_resource(pool_options(), __upstream)
    { }

    explicit
    unsynchronized_pool_resource(const pool_options& __opts)
    : unsynchronized_pool_resource(__opts, get_default_resource()) { }

    unsynchronized_pool_resource(const unsynchronized_pool_resource&) = delete;

    virtual ~unsynchronized_pool_resource();

    unsynchronized_pool_resource&
    operator=(const unsynchronized_pool_resource&) = delete;

    void release();

    [[__gnu__::__returns_nonnull__]]
    memory_resource*
    upstream_resource() const noexcept
    { return _M_impl.resource(); }

    pool_options options() const noexcept { return _M_impl._M_opts; }

  protected:
    void*
    do_allocate(size_t __bytes, size_t __alignment) override;

    void
    do_deallocate(void* __p, size_t __bytes, size_t __alignment) override;

    bool
    do_is_equal(const memory_resource& __other) const noexcept override
    { return this == &__other; }

  private:
    using _Pool = __pool_resource::_Pool;

    auto _M_find_pool(size_t) noexcept;

    __pool_resource _M_impl;
    _Pool* _M_pools = nullptr;
  };

  class monotonic_buffer_resource : public memory_resource
  {
  public:
    explicit
    monotonic_buffer_resource(memory_resource* __upstream) noexcept
    __attribute__((__nonnull__))
    : _M_upstream(__upstream)
    { _GLIBCXX_DEBUG_ASSERT(__upstream != nullptr); }

    monotonic_buffer_resource(size_t __initial_size,
			      memory_resource* __upstream) noexcept
    __attribute__((__nonnull__))
    : _M_next_bufsiz(__initial_size),
      _M_upstream(__upstream)
    {
      _GLIBCXX_DEBUG_ASSERT(__upstream != nullptr);
      _GLIBCXX_DEBUG_ASSERT(__initial_size > 0);
    }

    monotonic_buffer_resource(void* __buffer, size_t __buffer_size,
			      memory_resource* __upstream) noexcept
    __attribute__((__nonnull__(4)))
    : _M_current_buf(__buffer), _M_avail(__buffer_size),
      _M_next_bufsiz(_S_next_bufsize(__buffer_size)),
      _M_upstream(__upstream),
      _M_orig_buf(__buffer), _M_orig_size(__buffer_size)
    {
      _GLIBCXX_DEBUG_ASSERT(__upstream != nullptr);
      _GLIBCXX_DEBUG_ASSERT(__buffer != nullptr || __buffer_size == 0);
    }

    monotonic_buffer_resource() noexcept
    : monotonic_buffer_resource(get_default_resource())
    { }

    explicit
    monotonic_buffer_resource(size_t __initial_size) noexcept
    : monotonic_buffer_resource(__initial_size, get_default_resource())
    { }

    monotonic_buffer_resource(void* __buffer, size_t __buffer_size) noexcept
    : monotonic_buffer_resource(__buffer, __buffer_size, get_default_resource())
    { }

    monotonic_buffer_resource(const monotonic_buffer_resource&) = delete;

    virtual ~monotonic_buffer_resource() { release(); }

    monotonic_buffer_resource&
    operator=(const monotonic_buffer_resource&) = delete;

    void
    release() noexcept
    {
      if (_M_head)
	_M_release_buffers();

      // reset to initial state at contruction:
      if ((_M_current_buf = _M_orig_buf))
	{
	  _M_avail = _M_orig_size;
	  _M_next_bufsiz = _S_next_bufsize(_M_orig_size);
	}
      else
	{
	  _M_avail = 0;
	  _M_next_bufsiz = _M_orig_size;
	}
    }

    memory_resource*
    upstream_resource() const noexcept
    __attribute__((__returns_nonnull__))
    { return _M_upstream; }

  protected:
    void*
    do_allocate(size_t __bytes, size_t __alignment) override
    {
      if (__bytes == 0)
	__bytes = 1; // Ensures we don't return the same pointer twice.

      void* __p = std::align(__alignment, __bytes, _M_current_buf, _M_avail);
      if (!__p)
	{
	  _M_new_buffer(__bytes, __alignment);
	  __p = _M_current_buf;
	}
      _M_current_buf = (char*)_M_current_buf + __bytes;
      _M_avail -= __bytes;
      return __p;
    }

    void
    do_deallocate(void*, size_t, size_t) override
    { }

    bool
    do_is_equal(const memory_resource& __other) const noexcept override
    { return this == &__other; }

  private:
    // Update _M_current_buf and _M_avail to refer to a new buffer with
    // at least the specified size and alignment, allocated from upstream.
    void
    _M_new_buffer(size_t __bytes, size_t __alignment);

    // Deallocate all buffers obtained from upstream.
    void
    _M_release_buffers() noexcept;

    static size_t
    _S_next_bufsize(size_t __buffer_size) noexcept
    {
      if (__buffer_size == 0)
	__buffer_size = 1;
      return __buffer_size * _S_growth_factor;
    }

    static constexpr size_t _S_init_bufsize = 128 * sizeof(void*);
    static constexpr float _S_growth_factor = 1.5;

    void*	_M_current_buf = nullptr;
    size_t	_M_avail = 0;
    size_t	_M_next_bufsiz = _S_init_bufsize;

    // Initial values set at construction and reused by release():
    memory_resource* const	_M_upstream;
    void* const			_M_orig_buf = nullptr;
    size_t const		_M_orig_size = _M_next_bufsiz;

    class _Chunk;
    _Chunk* _M_head = nullptr;
  };

} // namespace pmr
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std