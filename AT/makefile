postFixCalc: main.cpp
	g++ -o postfixCalc main.cpp PostFixCalculator.cpp -std=c++0x

LIBS = -lboost_unit_test_framework

testexec : PostFixCalculator.cpp PostFixCalculator.t.cpp
	g++ -o testexec PostFixCalculator.cpp PostFixCalculator.t.cpp ${LIBS}

runtest:
	./testexec

test: testexec runtest

clean:
	$(RM) postfixCalc testexec
