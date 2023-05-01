#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#define MAKE_ENTRY_ID "make_entry"
#define RUN_ENTRY_ID "run_entry"
#define CU_ENTRY_ID "cu_entry"
#define CONFIG_SAVE_BUTTON_ID "config_save_button"


#define DEBUG_TEXTVIEW_ID "debugtextview"

#define SET_THREAD_OUTPUT_LINE_BUTTON_ID "set_thread_output_line_button"
#define SET_THREAD_OUTPUT_VALUE_BUTTON_ID "set_thread_output_value_button"
#define THREAD_OUTPUT_LINE_ENTRY_ID "thread_output_line_entry"
#define THREAD_OUTPUT_VALUE_ENTRY_ID "thread_output_value_entry"

#define SET_THREAD_OVERWRITE_LINE_BUTTON_ID "set_thread_overwrite_line_button"
#define SET_THREAD_OVERWRITE_VALUE_BUTTON_ID "set_thread_overwrite_value_button"
#define THREAD_OVERWRITE_LINE_ENTRY_ID "thread_overwrite_line_entry"
#define THREAD_OVERWRITE_VALUE_ENTRY_ID "thread_overwrite_value_entry"

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
  refBuilder->get_widget<Gtk::Entry>(THREAD_OUTPUT_VALUE_ENTRY_ID)->set_text(selected);
}

void set_thread_output_value(std::shared_ptr<Gtk::Builder> refBuilder) {
  
}

void init_config_page(std::shared_ptr<Gtk::Builder> refBuilder) {
  auto pConfigSaveButton = refBuilder->get_widget<Gtk::Button>(CONFIG_SAVE_BUTTON_ID);
  if (pConfigSaveButton) {
    pConfigSaveButton->signal_clicked().connect([refBuilder] () { save_config(refBuilder); });
  }
}

void init_debug_page(std::shared_ptr<Gtk::Builder> refBuilder) {
  // TODO: set thread output line number to cursor position
  
  // set thread output value entry to cursor selection
  auto pThreadOutputSetValueButton = refBuilder->get_widget<Gtk::Button>(SET_THREAD_OUTPUT_VALUE_BUTTON_ID);
  pThreadOutputSetValueButton->signal_clicked().connect([refBuilder] () {
    std::string selected = get_selected_text(refBuilder->get_widget<Gtk::TextView>(DEBUG_TEXTVIEW_ID));
    refBuilder->get_widget<Gtk::Entry>(THREAD_OUTPUT_VALUE_ENTRY_ID)->set_text(selected);
  });

   // thread output button
  auto pThreadOutputButton = refBuilder->get_widget<Gtk::Button>("thread_output_button");
  pThreadOutputButton->signal_clicked().connect([] () { system("src/scripts/threads.sh"); });

  // TODO: set thread overwrite line number to cursor position

  // set thread overwrite value entry to cursor selection
  auto pThreadOverwriteSetValueButton = refBuilder->get_widget<Gtk::Button>(SET_THREAD_OVERWRITE_VALUE_BUTTON_ID);
  pThreadOverwriteSetValueButton->signal_clicked().connect([refBuilder] () {
    std::string selected = get_selected_text(refBuilder->get_widget<Gtk::TextView>(DEBUG_TEXTVIEW_ID));
    refBuilder->get_widget<Gtk::Entry>(THREAD_OVERWRITE_VALUE_ENTRY_ID)->set_text(selected);
  });
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

  init_config_page(refBuilder);

  init_debug_page(refBuilder);

  // Get the GtkBuilder-instantiated button, and connect a signal handler:
  

  init_config(refBuilder);

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