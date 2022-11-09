with section("lint"):
  disabled_codes = [
    # A custom command with one output doesn't really need a comment because
    # the default "generating XXX" is a good message already.
    "C0113",
  ]


with section("format"):

  # Disable formatting entirely, making cmake-format a no-op
  disable = False

  # How wide to allow formatted cmake files
  line_width = 100

  # How many spaces to tab for indent
  tab_size = 4

  # If true, lines are indented using tab characters (utf-8 0x09) instead of
  # <tab_size> space characters (utf-8 0x20). In cases where the layout would
  # require a fractional tab character, the behavior of the  fractional
  # indentation is governed by <fractional_tab_policy>
  use_tabchars = False

  # If <use_tabchars> is True, then the value of this variable indicates how
  # fractional indentions are handled during whitespace replacement. If set to
  # 'use-space', fractional indentation is left as spaces (utf-8 0x20). If set
  # to `round-up` fractional indentation is replaced with a single tab character
  # (utf-8 0x09) effectively shifting the column to the next tabstop
  fractional_tab_policy = 'use-space'

  # If an argument group contains more than this many sub-groups (parg or kwarg
  # groups) then force it to a vertical layout.
  max_subgroups_hwrap = 4
  
  # If true, the parsers may infer whether or not an argument list is sortable
  # (without annotation).
  autosort = False
  
  # If a statement is wrapped to more than one line, than dangle the closing
  # parenthesis on its own line.
  dangle_parens = True

  # If the trailing parenthesis must be 'dangled' on its on line, then align it
  # to this reference: `prefix`: the start of the statement,  `prefix-indent`:
  # the start of the statement, plus one indentation  level, `child`: align to
  # the column of the arguments
  dangle_align = 'prefix'