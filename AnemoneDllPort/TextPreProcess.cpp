#include "stdafx.h"
#include "TextPreProcess.h"
#include <cassert>
#include <regex>
#include <array>
#include <functional>

static std::wstring PreProcessFunc_NameExistAndTextNotStartWithName(const std::wstring &text)
{
	// �Ϻ� ������ ���� ��ŷ�� �ؽ�Ʈ�� �߰��� �̸��� �����ִ�.
	// ��~�� �� ���� �߰��� ������ ���� ������ ó������ ���ġ
	// ��~�� �� �̸��� ǥ���ϴµ� ���Ǵ� Ư������
	std::wregex nameExistButNotFirstRegex(L"^.+(��.+��).*$");
	std::wsmatch m;
	if (std::regex_match(text, m, nameExistButNotFirstRegex))
	{
		if (m.size() >= 2)
		{
			int pos = m.position(1);
			int len = m.length(1);
			std::wstring bodyHead = text.substr(0, pos);
			std::wstring name = text.substr(pos, len);
			std::wstring bodyRemain = text.substr(pos + len, text.length() - pos - len);
			return (name + bodyHead + bodyRemain);
		}
	}
	return text;
}

CTextPreProcess::CTextPreProcess()
{

}

CTextPreProcess::~CTextPreProcess()
{

}

std::wstring CTextPreProcess::PreProcessText(const std::wstring &input, bool bForceOff)
{
	// ������ ���������� �ؽ�Ʈ �߰��� �̸��� �־ �����Ѵ�
	if (bForceOff) return input;

	// Rule 0. ������ �ʹ� ª���� ��ó���⸦ ������� �ʴ´�
	const int LOWER_BOUND_FOR_PRE_PROCESS = 2;
	if (input.empty() || input.length() <= LOWER_BOUND_FOR_PRE_PROCESS)
	{
		return input;
	}

	typedef std::function<std::wstring(std::wstring)> PreProcessFuncType;
	std::array<PreProcessFuncType, 1> funcs = {
		PreProcessFunc_NameExistAndTextNotStartWithName,
	};

	std::wstring text = input;
	for (auto &func : funcs)
	{
		text = func(text);
	}

	return text;
}

static int testMainForTextPreProcess()
{
	// ��ó���⸦ �����ϰ� �׽�Ʈ
	// �Ƴ׸�� ���� �ڵ忡�� �׽�Ʈ�� ���õ� �ڵ尡 ��� ������ ��������
	CTextPreProcess subject;
	{
		auto input = L"���⡼������?���ʪ�ʪ���諪�ޫ��ϡ���";
		auto expected = L"������������?���ʪ�ʪ���諪�ޫ��ϡ���";
		auto actual = subject.PreProcessText(input);
		assert(actual == expected);
	}
	{
		auto input = L"����?���ʪ�ʪ���諪�ޫ��ϡ�����������";
		auto expected = L"������������?���ʪ�ʪ���諪�ޫ��ϡ���";
		auto actual = subject.PreProcessText(input);
		assert(actual == expected);
	}
	{
		// ��ó������ �۾��� �������� �ʴ� ���
		std::array<std::wstring, 3> inputTexts = {
			L"������������?���ʪ�ʪ���諪�ޫ��ϡ���",	// �Ϲ� �԰� ���ڿ�
			L"123",	// �ʹ� ª��
			L"",	// ����ִ� 
		};
		for (auto &input : inputTexts)
		{
			auto actual = subject.PreProcessText(input);
			assert(actual == input);
		}
	}
	return 0;
}

#ifdef _DEBUG
static int __init_test_text_pre_process = testMainForTextPreProcess();
#endif


