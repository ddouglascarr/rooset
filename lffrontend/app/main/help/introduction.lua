ui.title(_"Introduction")

-- show the user what can be done
execute.view { module = "index", view = "_sidebar_whatcanido" }

ui.section(function()
  ui.sectionHead(function()
    ui.heading{ level = 1, content = _"Structured discussion" }
  end)
  ui.sectionRow(function()
    ui.heading{ level = 2, content = _"Initiatives and issues" }
    ui.tag{ tag = "p", content = _"[introduction] iniatives and issues" }
    ui.heading{ level = 2, content = _"Subject areas" }
    ui.tag{ tag = "p", content = _"[introduction] subject areas" }
    ui.heading{ level = 2, content = _"Organizational units" }
    ui.tag{ tag = "p", content = _"[introduction] organizational units" }
    ui.heading{ level = 2, content = _"Rules of procedure" }
    ui.tag{ tag = "p", content = _"[introduction] rules of procedure" }
  end )
end )
ui.section(function()
  ui.sectionHead(function()
    ui.heading{ level = 1, content = _"4 phases of a decision" }
  end)
  ui.sectionRow(function()
    ui.heading{ level = 2, content = _"(1) Admission phase" }
    ui.tag{ tag = "p", content = _"[introduction] phase 1 admission" }
    ui.heading{ level = 2, content = _"(2) Discussion phase" }
    ui.tag{ tag = "p", content = _"[introduction] phase 2 discussion" }
    ui.heading{ level = 2, content = _"(3) Verification phase" }
    ui.tag{ tag = "p", content = _"[introduction] phase 3 verification" }
    ui.heading{ level = 2, content = _"(4) Voting phase" }
    ui.tag{ tag = "p", content = _"[introduction] phase 4 voting" }
  end)
end)
ui.section(function()
  ui.sectionHead(function()
    ui.heading{ level = 1, content = _"Vote delegation" }
  end)
  ui.sectionRow(function()
    ui.tag{ tag = "p", content = _"[introduction] vote delegation" }
  end)
end)
ui.section(function()
  ui.sectionHead(function()
    ui.heading{ level = 1, content = _"Preference voting" }
  end)
  ui.sectionRow(function()
    ui.tag{ tag = "p", content = _"[introduction] preference voting" }
  end)
end)
