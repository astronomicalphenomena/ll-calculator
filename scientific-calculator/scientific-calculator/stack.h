#pragma once
template <typename T>
class Stack
{
public:
	bool empty()
	{
		return isEmpty;
	}
	void push(T data)
	{
		if (isEmpty)
		{
			top = new node;
			top->data = data;
			isEmpty = false;
		}
		else
		{
			node *temp = top;
			top = new node;
			top->data = data;
			top->last = temp;
		}
	}
	T GetTop()
	{
		if (isEmpty)
			throw "Stack is Empty";
		return top->data;
	}
	T pop()
	{
		if (isEmpty)
			throw "Stack is Empty";
		if (top->last == nullptr)
		{
			T data = top->data;
			delete top;
			isEmpty = true;
			return data;
		}
		else
		{
			T data = top->data;
			node *temp = top;
			top = top->last;
			delete temp;
			return data;
		}
	}
	~Stack()
	{
		while (true)
		{
			if (isEmpty)
				break;
			pop();
		}
	}
private:
	struct node
	{
		T data;
		node *last = nullptr;
	};
	node *top;
	bool isEmpty = true;
};