

CBase::CBase(void):
	m_dwCntRef(0)
{

}
CBase::~CBase(void)
{

}

unsigned long CBase::AddRef(void)
{
	return m_dwCntRef++;
}
unsigned long CBase::Release(void)
{
	if (0 == m_dwCntRef)
	{
		Free();

		delete this;

		return 0;
	}

	return m_dwCntRef--;
}