#include <stdio.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>

int suma(int a, int b);

int suma (int a, int b ) {
    return (a + b);
}

void test_suma () {
    CU_ASSERT_EQUAL(suma(0,2), 2);
}

int main(void)
{
    CU_initialize_registry();
    CU_pSuite prueba=CU_add_suite("Suitedeprueba",NULL,NULL);
    //CU_basic_set_mode();

    CU_add_test(prueba,"uno", test_suma);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}