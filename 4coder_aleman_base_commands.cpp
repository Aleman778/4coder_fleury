// My own custom commands that I personally like to have written by Alexander Mennborg.

CUSTOM_COMMAND_SIG(project_f1key_command) {
    prj_exec_command_fkey_index(app, 0);
}

function void
seek_pos_of_visual_line_or_blank_line(Application_Links *app, Side side,
                                      Scan_Direction direction, Position_Within_Line position) {
    View_ID view = get_active_view(app, Access_ReadVisible);
    i64 pos = view_get_cursor_pos(app, view);
    Buffer_Cursor cursor = view_compute_cursor(app, view, seek_pos(pos));
    Vec2_f32 p = view_relative_xy_of_pos(app, view, cursor.line, pos);
    p.x = (side == Side_Min)?(0.f):(max_f32);
    i64 new_pos = view_pos_at_relative_xy(app, view, cursor.line, p);

    if (new_pos == pos) {
        if (pos > 0) {
            seek_blank_line(app, direction, position);
        }
    } else {
        view_set_cursor_and_preferred_x(app, view, seek_pos(new_pos));
        no_mark_snap_to_cursor_if_shift(app, view);
    }
}

CUSTOM_COMMAND_SIG(seek_beginning_of_line_or_block) {
    seek_pos_of_visual_line_or_blank_line(app, Side_Min, Scan_Backward, PositionWithinLine_End);
}

CUSTOM_COMMAND_SIG(seek_end_of_line_or_block) {
    seek_pos_of_visual_line_or_blank_line(app, Side_Max, Scan_Forward, PositionWithinLine_Start);
}

CUSTOM_COMMAND_SIG(insert_newline) {
    write_text(app, string_u8_litexpr("\n"));
}

CUSTOM_COMMAND_SIG(append_newline) {
    insert_newline(app);
    move_left(app);
}
