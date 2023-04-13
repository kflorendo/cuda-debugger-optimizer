#ifndef GUI_H
#define GUI_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>

class Gui : public Gtk::Window
{

public:
  Gui();
  ~Gui() override;

protected:
  //Signal handlers:
  void on_button_clicked();

  //Member widgets:
  Gtk::Button m_button;
};

#endif // GUI_H