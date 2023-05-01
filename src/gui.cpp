#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#define MAKE_ENTRY_ID "make_entry"
#define RUN_ENTRY_ID "run_entry"
#define CU_ENTRY_ID "cu_entry"
#define DEBUG_TEXTVIEW_ID "debugtextview"
#define SET_THREAD_OUTPUT_LINE_BUTTON_ID "set_thread_output_line_button"
#define SET_THREAD_OUTPUT_VALUE_BUTTON_ID "set_thread_output_value_button"
#define THREAD_OUTPUT_LINE_ENTRY_ID "thread_output_line_entry"
#define THREAD_OUTPUT_VALUE_ENTRY_ID "thread_output_value_entry"

namespace
{
Gtk::Window* pWindow = nullptr;
Glib::RefPtr<Gtk::Application> app;

void set_text_entry(std::shared_ptr<Gtk::Builder> refBuilder, std::string id, std::string text)
{
  auto pEntry = refBuilder->get_widget<Gtk::Entry>(id);
  if (pEntry)
    pEntry->set_text(text);
}

void save_config(std::shared_ptr<Gtk::Builder> refBuilder) {
  auto pCuEntry = refBuilder->get_widget<Gtk::Entry>(CU_ENTRY_ID);
  auto pDebugTextView = refBuilder->get_widget<Gtk::TextView>(DEBUG_TEXTVIEW_ID);
  if (pDebugTextView) {
    std::ifstream cuFile(pCuEntry->get_text());
    std::stringstream fileContents;
    fileContents << cuFile.rdbuf();
    pDebugTextView->get_buffer()->set_text(fileContents.str());
    cuFile.close();
  }
}

void init_config(std::shared_ptr<Gtk::Builder> refBuilder) {
  // Create a text string, which is used to output the text file
  std::string configText;

  // Read from the text file
  std::ifstream configReadFile("output/config.txt");

  getline(configReadFile, configText);
  set_text_entry(refBuilder, MAKE_ENTRY_ID, configText);

  getline(configReadFile, configText);
  set_text_entry(refBuilder, RUN_ENTRY_ID, configText);

  getline(configReadFile, configText);
  set_text_entry(refBuilder, CU_ENTRY_ID, configText);

  // Close the file
  configReadFile.close();

  save_config(refBuilder);
}

void on_thread_output_button_clicked()
{
  system("src/scripts/threads.sh");
}

std::string get_selected_text(Gtk::TextView *textView) {
  Gtk::TextBuffer::iterator range_start;
  Gtk::TextBuffer::iterator range_end;
  if (textView->get_buffer()->get_selection_bounds(range_start, range_end)) {
    return textView->get_buffer()->get_text(range_start, range_end, true);
  } else {
    return "";
  }
}

void set_thread_output_line(std::shared_ptr<Gtk::Builder> refBuilder) {
  std::string selected = get_selected_text(refBuilder->get_widget<Gtk::TextView>(DEBUG_TEXTVIEW_ID));
  std::cerr << selected << std::endl;
  refBuilder->get_widget<Gtk::Entry>(THREAD_OUTPUT_VALUE_ENTRY_ID)->set_text(selected);
}

void set_thread_output_value(std::shared_ptr<Gtk::Builder> refBuilder) {
  std::string selected = get_selected_text(refBuilder->get_widget<Gtk::TextView>(DEBUG_TEXTVIEW_ID));
  std::cerr << selected << std::endl;
  refBuilder->get_widget<Gtk::Entry>(THREAD_OUTPUT_VALUE_ENTRY_ID)->set_text(selected);
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

  init_config(refBuilder);

  // Get the GtkBuilder-instantiated button, and connect a signal handler:
  auto pButton = refBuilder->get_widget<Gtk::Button>("thread_output_button");
  if (pButton)
    pButton->signal_clicked().connect([] () { on_thread_output_button_clicked(); });

  auto pThreadOutputSetLineButton = refBuilder->get_widget<Gtk::Button>(SET_THREAD_OUTPUT_LINE_BUTTON_ID);
  if (pThreadOutputSetLineButton)
    pThreadOutputSetLineButton->signal_clicked().connect([refBuilder] () { set_thread_output_line(refBuilder); });
  
  auto pThreadOutputSetValueButton = refBuilder->get_widget<Gtk::Button>(SET_THREAD_OUTPUT_VALUE_BUTTON_ID);
  if (pThreadOutputSetValueButton)
    pThreadOutputSetValueButton->signal_clicked().connect([refBuilder] () { set_thread_output_value(refBuilder); });

  auto pConfigSaveButton = refBuilder->get_widget<Gtk::Button>("config_save_button");
  if (pConfigSaveButton) {
    pConfigSaveButton->signal_clicked().connect([refBuilder] () { save_config(refBuilder); });
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