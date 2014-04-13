int fun()
{
	float a=2.3,b=3.2;
	a=b/2.1-a;
	return a;
}
main()
{
	int b;
	int a;
	b=fun();
	a=b==3?b-2:1;
}
