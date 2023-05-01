#include <gtkmm.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

namespace
{
Gtk::Window* pWindow = nullptr;
Glib::RefPtr<Gtk::Application> app;

void on_thread_output_button_clicked()
{
  system("src/scripts/threads.sh");
}

void on_config_save_button_clicked(std::shared_ptr<Gtk::Builder> refBuilder) {
  auto pDebugTextView = refBuilder->get_widget<Gtk::TextView>("debugtextview");
  if (pDebugTextView)
    pDebugTextView->get_buffer()->set_text("this worked!");
}


void on_app_activate()
{
  // Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("src/gui4.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return;
  }

  // Get the GtkBuilder-instantiated window:
  pWindow = refBuilder->get_widget<Gtk::Window>("window");
  if (!pWindow)
  {
    std::cerr << "Could not get the window" << std::endl;
    return;
  }

  // Get the GtkBuilder-instantiated button, and connect a signal handler:
  auto pButton = refBuilder->get_widget<Gtk::Button>("thread_output_button");
  if (pButton)
    pButton->signal_clicked().connect([] () { on_thread_output_button_clicked(); });

  auto pConfigSaveButton = refBuilder->get_widget<Gtk::Button>("config_save_button");
  if (pConfigSaveButton) {
    pConfigSaveButton->signal_clicked().connect([refBuilder] () { on_config_save_button_clicked(refBuilder); });
  }

  // It's not possible to delete widgets after app->run() has returned.
  // Delete the window with its child widgets before app->run() returns.
  pWindow->signal_hide().connect([] () { delete pWindow; });

  app->add_window(*pWindow);
  pWindow->set_visible(true);
}

} // anonymous namespace

int main(int argc, char** argv)
{
  app = Gtk::Application::create("org.gtkmm.example");

  // Instantiate a dialog when the application has been activated.
  // This can only be done after the application has been registered.
  // It's possible to call app->register_application() explicitly, but
  // usually it's easier to let app->run() do it for you.
  app->signal_activate().connect([] () { on_app_activate(); });

  return app->run(argc, argv);
}


// Gui::Gui()
// : m_button("Hello World")   // creates a new button with label "Hello World".
// {
//   set_title("Basic application");
//   set_default_size(200, 200);

//   // Sets the margin around the button.
//   m_button.set_margin(10);

//   // When the button receives the "clicked" signal, it will call the
//   // on_button_clicked() method defined below.
//   m_button.signal_clicked().connect(sigc::mem_fun(*this,
//               &Gui::on_button_clicked));

//   // This packs the button into the Window (a container).
//   set_child(m_button);
// }

// Gui::~Gui()
// {
// }

// void Gui::on_button_clicked()
// {
//   std::cout << "Hello World" << std::endl;
//   system("src/scripts/threads.sh");
// }