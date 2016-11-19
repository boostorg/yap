void compile_const_term();
void compile_copy_only_types();
void compile_move_only_types();
void compile_placeholders();
void compile_term_plus_expr();
void compile_term_plus_term();
void compile_term_plus_x();
void compile_term_plus_x_this_ref_overloads();
void compile_x_plus_term();

int main ()
{
    compile_const_term();
    compile_copy_only_types();
    compile_move_only_types();
    compile_placeholders();
    compile_term_plus_expr();
    compile_term_plus_term();
    compile_term_plus_x();
    compile_term_plus_x_this_ref_overloads();
    compile_x_plus_term();
}
