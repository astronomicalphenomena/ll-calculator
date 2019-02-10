#pragma once
template <typename T>
class Stack
{
private:
	struct node
	{
		T data;
		node *next = nullptr;
	};
	node *_top;
	bool _isEmpty = true;

public:
	bool empty()
	{
		return _isEmpty;
	}
	void push(T data)
	{
		if (_isEmpty)
		{
			_top = new node;
			_top->data = data;
			_isEmpty = false;
		}
		else
		{
			node *temp = _top;
			_top = new node;
			_top->data = data;
			_top->next = temp;
		}
	}
	T GetTop()
	{
		return _top->data;
	}
	T pop()
	{
		if (_top->next == nullptr)
		{
			T data = _top->data;
			delete _top;
			_isEmpty = true;
			return data;
		}
		else
		{
			T data = _top->data;
			node *temp = _top;
			_top = _top->next;
			delete temp;
			return data;
		}
	}
	~Stack()
	{
		while (true)
		{
			if (_isEmpty)
				break;
			pop();
		}
	}
};