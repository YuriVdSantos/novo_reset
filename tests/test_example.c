#include <cmocka.h>

void test_example(void **state) {
    (void) state; 
    assert_int_equal(1 + 1, 2);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_example),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}