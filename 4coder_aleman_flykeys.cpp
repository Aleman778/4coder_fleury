// My (slightly modified) version of Xah flykeys in 4coder written by Alexander Mennborg
// Uses space as leader key, and following that you have a set of commands and keymaps.
// Keymaps are just a set of commands that can be accessed only once you have entered a map.
// The goal is to never require you to press controll or alt key which can be more ergonomic
// and put less strain on you hand.

enum Editor_Mode {
    EditorMode_Command,
    EditorMode_Insert,
    EditorMode_Visual,
    EditorMode_Leader,
    EditorMode_Keymap_I,
    EditorMode_Keymap_K,
    EditorMode_Keymap_D,
    EditorMode_Keymap_W,
    
    EditorMode_Count,
};

global bool global_enable_flykeys = false;
global Editor_Mode global_editor_mode = EditorMode_Command;
global Command_Map_ID global_command_map_ids[EditorMode_Count] = {};

CUSTOM_COMMAND_SIG(to_command_mode) {
    global_editor_mode = EditorMode_Command;
}

CUSTOM_COMMAND_SIG(to_insert_mode) {
    global_editor_mode = EditorMode_Insert;
}

CUSTOM_COMMAND_SIG(to_visual_mode) {
    global_editor_mode = EditorMode_Visual;
    set_mark(app);
}

CUSTOM_COMMAND_SIG(to_leader_mode) {
    global_editor_mode = EditorMode_Leader;
}

CUSTOM_COMMAND_SIG(to_i_keymap) {
    global_editor_mode = EditorMode_Keymap_I;
}

CUSTOM_COMMAND_SIG(to_k_keymap) {
    global_editor_mode = EditorMode_Keymap_K;
}

CUSTOM_COMMAND_SIG(to_d_keymap) {
    global_editor_mode = EditorMode_Keymap_D;
}

function Implicit_Map_Result
F4_ImplicitMap(Application_Links *app, String_ID lang, String_ID mode, Input_Event *event) {
    Implicit_Map_Result result = {};
    
    Command_Map_ID map_id = global_command_map_ids[global_editor_mode];
    Command_Binding binding = map_get_binding_recursive(&framework_mapping, map_id, event);

    // if(!binding.custom) {
    //     Command_Map_ID global_map_id = (Command_Map_ID) vars_save_string_lit("keys_shared");
    //     // View_ID view = get_this_ctx_view(app, Access_Always);
    //     // Command_Map_ID orig_id = default_get_map_id(app, view);
    //     binding = map_get_binding_recursive(&framework_mapping, global_map_id, event);
    // }

    result.map = 0;
    result.command = binding.custom;
    return result;
}


void
F4_SetAbsolutelyNecessaryBindings(Mapping* mapping) {
    String_ID global_map_id = vars_save_string_lit("keys_global");
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");

    implicit_map_function = F4_ImplicitMap;

    MappingScope();
    SelectMapping(mapping);
    
    SelectMap(global_map_id);
    BindCore(fleury_startup, CoreCode_Startup);
    BindCore(default_try_exit, CoreCode_TryExit);
    Bind(exit_4coder,          KeyCode_F4, KeyCode_Alt);
    BindMouseWheel(mouse_wheel_scroll);
    BindMouseWheel(mouse_wheel_change_face_size, KeyCode_Control);
    
    SelectMap(file_map_id);
    ParentMap(global_map_id);
    BindMouse(click_set_cursor_and_mark, MouseCode_Left);
    BindMouseRelease(click_set_cursor, MouseCode_Left);
    BindCore(click_set_cursor_and_mark, CoreCode_ClickActivateView);
    BindMouseMove(click_set_cursor_if_lbutton);
    
    SelectMap(code_map_id);
    ParentMap(file_map_id);
    BindMouse(f4_lego_click_store_token_1, MouseCode_Right);
    BindMouse(f4_lego_click_store_token_2, MouseCode_Middle);
}

function void
F4_SetDefaultBindings(Mapping *mapping) {
    String_ID file_map_id = vars_save_string_lit("keys_file");

    MappingScope();
    SelectMapping(mapping);

    // NOTE(alexander): keys that are shared for both command and insert mode
    String_ID shared_map_id = vars_save_string_lit("keys_shared");
    SelectMap(shared_map_id);
    ParentMap(file_map_id);
    Bind(to_command_mode, KeyCode_Escape);
    Bind(word_complete, KeyCode_Tab);
    Bind(delete_char, KeyCode_Delete);
    Bind(backspace_char, KeyCode_Backspace);
    Bind(move_left, KeyCode_Left);
    Bind(move_right, KeyCode_Right);
    Bind(move_up, KeyCode_Up);
    Bind(move_down, KeyCode_Down);
    Bind(project_fkey_command, KeyCode_F1);
    Bind(project_fkey_command, KeyCode_F2);
    Bind(project_fkey_command, KeyCode_F3);
    Bind(project_fkey_command, KeyCode_F4);
    Bind(project_fkey_command, KeyCode_F5);
    Bind(project_fkey_command, KeyCode_F6);
    Bind(project_fkey_command, KeyCode_F7);
    Bind(project_fkey_command, KeyCode_F8);
    Bind(project_fkey_command, KeyCode_F9);
    Bind(project_fkey_command, KeyCode_F10);
    Bind(project_fkey_command, KeyCode_F11);
    Bind(project_fkey_command, KeyCode_F12);
    Bind(project_fkey_command, KeyCode_F13);
    Bind(project_fkey_command, KeyCode_F14);
    Bind(project_fkey_command, KeyCode_F15);
    Bind(project_fkey_command, KeyCode_F16);

    // NOTE(alexander): keys for command mode
    String_ID command_mode_map_id = vars_save_string_lit("command_mode");
    global_command_map_ids[EditorMode_Command] = command_mode_map_id;
    SelectMap(command_mode_map_id);
    ParentMap(shared_map_id);
    Bind(to_leader_mode, KeyCode_Space);
    Bind(to_leader_mode, KeyCode_Backspace);
    Bind(to_insert_mode, KeyCode_F);
    Bind(to_visual_mode, KeyCode_T);
    Bind(insert_newline, KeyCode_Return);
    // Bind(if_read_only_goto_position,  KeyCode_Return);
    Bind(close_panel, KeyCode_3);
    Bind(open_panel_hsplit, KeyCode_4);
    Bind(command_lister, KeyCode_A);
    Bind(snipe_backward_whitespace_or_token_boundary, KeyCode_E);
    Bind(snipe_forward_whitespace_or_token_boundary, KeyCode_R);
    Bind(delete_char, KeyCode_5);
    Bind(delete_line, KeyCode_G);
    Bind(backspace_char, KeyCode_D);
    Bind(center_view,  KeyCode_U);
    Bind(move_up, KeyCode_O);
    Bind(move_left, KeyCode_K);
    Bind(move_down, KeyCode_L);
    Bind(move_right, KeyCode_Semicolon);
    Bind(move_left_alpha_numeric_or_camel_boundary,  KeyCode_I);
    Bind(move_right_alpha_numeric_or_camel_boundary, KeyCode_P);
    Bind(goto_prev_jump, KeyCode_H);
    Bind(seek_beginning_of_line_or_block,  KeyCode_J);
    Bind(seek_end_of_line_or_block, KeyCode_Quote);
    Bind(goto_next_jump, KeyCode_BackwardSlash);
    Bind(comment_line_toggle, KeyCode_Z);
    Bind(append_newline,  KeyCode_S);
    Bind(cut, KeyCode_X);
    Bind(copy, KeyCode_C);
    Bind(paste, KeyCode_V);
    Bind(undo, KeyCode_Y);
    Bind(redo, KeyCode_B);
    Bind(search, KeyCode_N);
    Bind(change_active_panel, KeyCode_Comma);
    Bind(change_active_panel_backwards, KeyCode_Period);

    // NOTE(alexander): keys for insert mode
    String_ID insert_mode_map_id = vars_save_string_lit("insert_mode");
    global_command_map_ids[EditorMode_Insert] = insert_mode_map_id;
    SelectMap(insert_mode_map_id);
    ParentMap(shared_map_id);
    BindTextInput(f4_write_text_and_auto_indent);
    Bind(to_command_mode, KeyCode_Home);

    // NOTE(alexander): keys available after pressing leader key
    String_ID leader_mode_map_id = vars_save_string_lit("leader_mode");
    global_command_map_ids[EditorMode_Leader] = leader_mode_map_id;
    SelectMap(leader_mode_map_id);
    ParentMap(file_map_id);
    Bind(to_command_mode, KeyCode_Escape);
    Bind(to_insert_mode, KeyCode_Backspace);
    Bind(to_insert_mode, KeyCode_Space);
    Bind(to_i_keymap, KeyCode_I);
    Bind(to_k_keymap, KeyCode_K);
    Bind(to_d_keymap, KeyCode_D);
    
    Bind(close_panel, KeyCode_3);
    Bind(open_panel_vsplit, KeyCode_4);
    Bind(select_all, KeyCode_A);
    Bind(query_replace, KeyCode_R);
    Bind(goto_beginning_of_file, KeyCode_H);
    Bind(goto_end_of_file, KeyCode_N);
    Bind(project_f1key_command, KeyCode_P);
    Bind(save_all_dirty_buffers, KeyCode_Semicolon);
    BindTextInput(to_command_mode);

    // NOTE(alexander): keys for i keymap
    String_ID keymap_i_map_id = vars_save_string_lit("i_keymap");
    global_command_map_ids[EditorMode_Keymap_I] = keymap_i_map_id;
    SelectMap(keymap_i_map_id);
    ParentMap(file_map_id);
    Bind(to_command_mode, KeyCode_Escape);
    Bind(interactive_open_or_new, KeyCode_E);
    Bind(interactive_switch_buffer, KeyCode_D);
    Bind(open_matching_file_cpp, KeyCode_C);
    BindTextInput(to_command_mode);

    // NOTE(alexander): keys for k keymap
    String_ID keymap_k_map_id = vars_save_string_lit("k_keymap");
    global_command_map_ids[EditorMode_Keymap_K] = keymap_k_map_id;
    SelectMap(keymap_k_map_id);
    ParentMap(file_map_id);
    Bind(to_command_mode, KeyCode_Escape);
    Bind(kill_buffer, KeyCode_J);
    Bind(open_matching_file_cpp, KeyCode_C);
    Bind(goto_line, KeyCode_Y);
    Bind(query_replace_identifier, KeyCode_R);
    Bind(query_replace_selection, KeyCode_T);
    BindTextInput(to_command_mode);

    // NOTE(alexander): keys for d keymap
    String_ID keymap_d_map_id = vars_save_string_lit("d_keymap");
    global_command_map_ids[EditorMode_Keymap_D] = keymap_d_map_id;
    SelectMap(keymap_d_map_id);
    ParentMap(file_map_id);
    Bind(write_todo, KeyCode_T);
    Bind(write_note, KeyCode_N);
    BindTextInput(to_command_mode);
 
    // NOTE(alexander): setup global state
    global_enable_flykeys = true;
    global_editor_mode = EditorMode_Command;
}
