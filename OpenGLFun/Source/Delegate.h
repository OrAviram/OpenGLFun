#pragma once

template<typename TRet, typename... TArgs>
class Delegate
{
public:
	Delegate() : object(nullptr), method(nullptr) { }
	Delegate(const Delegate& other) : object(other.object), method(other.method) { }

	bool IsNull() { return method == nullptr; }

	template<typename TObj, TRet(TObj::*TFunc)(TArgs...)>
	static Delegate FromMethod(TObj* object)
	{
		Delegate d;
		d.object = object;
		d.method = &CallMethod<TObj, TFunc>;
		return d;
	}

	template<TRet(*TFunc)(TArgs...)>
	static Delegate FromFunction()
	{
		Delegate d;
		d.method = &CallFunction<TFunc>;
		return d;
	}

	TRet Invoke(TArgs... args) const
	{
		return method(object, args...);
	}

	TRet operator()(TArgs... args) const
	{
		return method(object, args...);
	}

	bool operator==(Delegate other) { return other.object == object && other.method == method; }
	bool operator!=(Delegate other) { return other.object != object || other.method != method; }

private:
	typedef TRet(*MethodType)(void* obj, TArgs... args);

	void* object;
	MethodType method;

	template<typename TObj, TRet(TObj::*TFunc)(TArgs...)>
	static TRet CallMethod(void* obj, TArgs... args)
	{
		TObj* object = static_cast<TObj*>(obj);
		return (object->*TFunc)(args...);
	}

	template<TRet(*TFunc)(TArgs...)>
	static TRet CallFunction(void* obj, TArgs... args)
	{
		return TFunc(args...);
	}
};

template<typename TRet, typename... TArgs>
class DelegateChain
{
public:
	typedef Delegate<TRet, TArgs...> Del;

private:
	struct Node
	{
		Node(Del value) : value(value), next(nullptr) { }

		Del value;
		Node* next;
	};

	Node* root;

public:
	DelegateChain() : root(nullptr) { }
	DelegateChain(Del delegate) : root(new Node(delegate)) { }
	DelegateChain(DelegateChain&& other) : root(other.root) { other.root = nullptr; }
	void operator=(DelegateChain&& other)
	{
		root = other.root;
		other.root = nullptr;
	}

	inline operator bool() { return root != nullptr; }

	static void Duplicate(const DelegateChain& source, DelegateChain& target)
	{
		Node* node = source.root;
		Node** duplicateNode = &target.root;
		while (node)
		{
			*duplicateNode = new Node(node->value);
			duplicateNode = &(*duplicateNode)->next;
			node = node->next;
		}
	}

	static DelegateChain Duplicate(const DelegateChain& other)
	{
		DelegateChain duplicate;
		Duplicate(other, duplicate);
		return duplicate;
	}

	~DelegateChain()
	{
		Node* current = root;
		while (current)
		{
			Node* next = current->next;
			delete current;
			current = next;
		}
	}

	void Add(Del delegate)
	{
		if (!root)
		{
			root = new Node(delegate);
			return;
		}

		Node* last = root;
		while (last)
		{
			if (!last->next)
			{
				last->next = new Node(delegate);
				return;
			}
			last = last->next;
		}
	}

	inline void operator+=(Del other) { Add(other); }

	void Append(const DelegateChain& other)
	{
		if (!root)
			Duplicate(other, *this);
		else
		{
			Node* originalRoot = root;
			Node* lastNode = root;
			while (lastNode->next)
				lastNode = lastNode->next;

			Duplicate(other, *this);
			lastNode->next = root;
			root = originalRoot;
		}
	}

	inline void operator+=(const DelegateChain& other) { Append(other); }

	void Remove(Del delegate)
	{
		if (!root)
			return;

		Node* oneBeforeTarget = nullptr;
		Node* iterator = root;
		while (iterator)
		{
			if (iterator->next)
			{
				if (iterator->next->value == delegate)
					oneBeforeTarget = iterator;
			}
			iterator = iterator->next;
		}

		if (oneBeforeTarget)
			RemoveNode(oneBeforeTarget);
		else if (root->value == delegate)
			RemoveRoot();
	}

	inline void operator-=(Del delegate) { Remove(delegate); }

	inline TRet Invoke(TArgs... args) const
	{
		return (*this)(args...);
	}

	TRet operator()(TArgs... args) const
	{
		ASSERT(root);

		Node* current = root;
		while (current->next)
		{
			current->value(args...);
			current = current->next;
		}
		return current->value(args...);

		// Commented out implementation doesn't work when return value is void.
		//TRet ret = root->value(args...);
		//Node* current = root->next;
		//while (current)
		//{
		//	ret = current->value(args...);
		//	current = current->next;
		//}
		//return ret;
	}

private:
	void RemoveRoot()
	{
		Node* newRoot = root->next;
		delete root;
		root = newRoot;
	}

	void RemoveNode(Node* oneBeforeTarget)
	{
		Node* target = oneBeforeTarget->next;
		oneBeforeTarget->next = target->next;
		delete target;
	}

public:
	struct Iterator
	{
		Node* current;
		Iterator(Node* value) : current(value) { }
		static const Iterator Null;

		inline operator Del() { return current->value; }

		inline Del operator*() { return current->value; }

		inline Iterator operator++(int)
		{
			Iterator* ret = current;
			current = current->next;
			return ret;
		}

		inline Iterator operator++()
		{
			current = current->next;
			return *this;
		}

		inline bool operator==(Iterator other)
		{
			return current == other.current;
		}

		inline bool operator!=(Iterator other)
		{
			return current != other.current;
		}
	};

	Iterator begin()
	{
		return root;
	}

	Iterator end()
	{
		return nullptr;
	}
};

// Tests.
/*
struct Fun
{
	int value;
	const char* name;
	int Print(int multiplier)
	{
		std::cout << "Hello from " << name << std::endl;
		return value * multiplier;
	}

	static int Static(int rareresrsr)
	{
		std::cout << "Hello from static" << std::endl;
		return -rareresrsr;
	}
};

typedef Delegate<int, int> FunDelegate;
typedef DelegateChain<int, int> FunDelegateChain;

typedef Delegate<void, int> TestDel;
typedef DelegateChain<void, int> TestChain;

void Yayers(int i) { std::cout << "Yayers " << i << std::endl; }

void main()
{
	Fun f1;
	f1.name = "f1";
	f1.value = 3;
	
	Fun f2;
	f2.name = "f2";
	f2.value = 5;

	TestChain tors = TestDel::FromFunction<&Yayers>();
	tors(3);
	tors.Invoke(5);
	std::cin.get();

	//while (true)
	{
		DelegateChain chain = FunDelegate::FromMethod<Fun, &Fun::Print>(&f1);
		std::cout << chain(2) << std::endl; // 6 with f1
		std::cout << chain(3) << std::endl; // 9 with f1
		std::cout << std::endl;

		chain += FunDelegate::FromMethod<Fun, &Fun::Print>(&f2);
		std::cout << chain(2) << std::endl; // 10 with f1, f2
		std::cout << chain(3) << std::endl; // 15 with f1, f2
		std::cout << std::endl;
		
		chain -= FunDelegate::FromMethod<Fun, &Fun::Print>(&f1);
		std::cout << chain(3) << std::endl; // 15 with f2
		std::cout << std::endl;

		chain += FunDelegate::FromFunction<&Fun::Static>();
		std::cout << chain(2) << std::endl; // -2 with f2, static
		std::cout << chain(3) << std::endl; // -3 with f2, static
		std::cout << std::endl;
		
		chain += FunDelegate::FromMethod<Fun, &Fun::Print>(&f1);
		std::cout << chain(2) << std::endl; // 6 with f2, static, f1
		std::cout << std::endl;

		chain -= FunDelegate::FromFunction<&Fun::Static>();
		std::cout << chain(2) << std::endl; // 6 with f2, f1
		std::cout << std::endl;

		chain += FunDelegate::FromMethod<Fun, &Fun::Print>(&f1);
		std::cout << chain(2) << std::endl; // 6 with f2, f1, f1
		std::cout << std::endl;

		chain += FunDelegate::FromFunction<&Fun::Static>();
		std::cout << chain(2) << std::endl; // -2 with f2, f1, f1, static
		std::cout << std::endl;

		chain -= FunDelegate::FromFunction<&Fun::Static>();
		std::cout << chain(2) << std::endl; // 6 with f2, f1, f1
		std::cout << std::endl;

		chain += FunDelegate::FromMethod<Fun, &Fun::Print>(&f2);
		std::cout << chain(2) << std::endl; // 10 with f2, f1, f1, f2
		std::cout << std::endl;

		chain += FunDelegate::FromFunction<&Fun::Static>();
		std::cout << chain(2) << std::endl; // -2 with f2, f1, f1, f2, static
		std::cout << std::endl;

		chain -= FunDelegate::FromMethod<Fun, &Fun::Print>(&f2);
		std::cout << chain(2) << std::endl; // -2 with f2, f1, f1, static
		std::cout << std::endl;

		chain += FunDelegate::FromMethod<Fun, &Fun::Print>(&f2);
		std::cout << chain(2) << std::endl; // 10 with f2, f1, f1, static, f2
		std::cout << std::endl;

		chain -= FunDelegate::FromMethod<Fun, &Fun::Print>(&f1);
		std::cout << chain(2) << std::endl; // 10 with f2, f1, static, f2
		std::cout << std::endl;

		std::vector<int> rets;
		for (FunDelegate del : chain)
			rets.push_back(del(2));

		std::cout << std::endl;
		for (int i = 0; i < rets.size(); i++)
			std::cout << rets[i] << std::endl;

		{
			FunDelegateChain copy = FunDelegateChain::Duplicate(chain);
			std::cout << copy(5) << std::endl;
			std::cout << std::endl;

			copy += FunDelegate::FromMethod<Fun, &Fun::Print>(&f1);
			std::cout << copy(5) << std::endl;
			std::cout << std::endl;
		}

		std::cout << chain(1) << std::endl;

		{
			DelegateChain chain2 = FunDelegate::FromFunction<&Fun::Static>();
			chain2 += chain;
			
			chain(1);
			std::cout << std::endl;
			chain(2);
			std::cout << std::endl;
			chain(3);
			std::cout << std::endl;
			chain(4);
			std::cout << std::endl;
			chain2(1);
			std::cout << std::endl;
			chain2(2);
			std::cout << std::endl;
			chain2(3);
			std::cout << std::endl;
			chain2(4);
			std::cout << std::endl;
		}
		chain(1);
		std::cout << std::endl;
		chain(2);
		std::cout << std::endl;
		chain(3);
		std::cout << std::endl;
	}

	//FunDelegate del1 = FunDelegate::FromMethod<Fun, &Fun::Print>(&f1);
	//FunDelegate del2 = FunDelegate::FromMethod<Fun, &Fun::Print>(&f2);
	//FunDelegate del3 = FunDelegate::FromFunction<&Fun::Static>();
	//
	//std::cout << del1(2) << std::endl; // 6
	//std::cout << del1(3) << std::endl; // 9
	//std::cout << del2(2) << std::endl; // 10
	//std::cout << del2(3) << std::endl; // 15
	//std::cout << del3(2) << std::endl; // -2
	//std::cout << del3(3) << std::endl; // -3
	//
	std::cin.get();
	return;
}
*/