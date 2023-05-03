#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#define CONFIG_FILENAME "output/config.txt"

#define CONFIG_MAKE_ENTRY_ID "make_entry"
#define CONFIG_RUN_ENTRY_ID "run_entry"
#define CONFIG_CU_ENTRY_ID "cu_entry"
#define CONFIG_SAVE_BUTTON_ID "config_save_button"


#define DEBUG_TEXTVIEW_ID "debug_textview"

#define SET_THREAD_OUTPUT_LINE_BUTTON_ID "set_thread_output_line_button"
#define SET_THREAD_OUTPUT_VALUE_BUTTON_ID "set_thread_output_value_button"
#define THREAD_OUTPUT_LINE_ENTRY_ID "thread_output_line_entry"
#define THREAD_OUTPUT_VALUE_ENTRY_ID "thread_output_value_entry"
#define THREAD_OUTPUT_VALUE_TYPE_ENTRY_ID "thread_output_value_type_entry"
#define THREAD_OUTPUT_TREEVIEW_ID "thread_output_treeview"
#define THREAD_OUTPUT_SUBMIT_BUTTON_ID "thread_output_button"

#define SET_THREAD_OVERWRITE_LINE_BUTTON_ID "set_thread_overwrite_line_button"
#define SET_THREAD_OVERWRITE_VALUE_BUTTON_ID "set_thread_overwrite_value_button"
#define THREAD_OVERWRITE_LINE_ENTRY_ID "thread_overwrite_line_entry"
#define THREAD_OVERWRITE_VALUE_ENTRY_ID "thread_overwrite_value_entry"
#define THREAD_OVERWRITE_SUBMIT_BUTTON_ID "thread_overwrite_button"

#define OPT_CONFIG_TEXTVIEW_ID "opt_config_textview"

#define OPT_CONFIG_DIM_CHECKBOX_ID_PREFIX "opt_config_dim_checkbox"
#define OPT_CONFIG_DIM_ENTRY_ID_PREFIX "opt_config_dim_entry"
#define SET_OPT_CONFIG_DIM_BUTTON_ID_PREFIX "set_opt_config_dim_button"
#define OPT_CONFIG_DIM_START_ENTRY_ID_PREFIX "opt_config_dim_start_entry"
#define OPT_CONFIG_DIM_STOP_ENTRY_ID_PREFIX "opt_config_dim_stop_entry"
#define OPT_CONFIG_DIM_STEP_ENTRY_ID_PREFIX "opt_config_dim_step_entry"

namespace
{
Gtk::Window* pWindow = nullptr;
Glib::RefPtr<Gtk::Application> app;

Glib::RefPtr<Gtk::Builder> refBuilder;

class ThreadOutputColumns : public Gtk::TreeModel::ColumnRecord
{
public:

  ThreadOutputColumns() {
    add(m_col_block_x);
    add(m_col_block_y);
    add(m_col_block_z);
    add(m_col_thread_x);
    add(m_col_thread_y);
    add(m_col_thread_z);
    add(m_col_value);
  }

  Gtk::TreeModelColumn<unsigned int> m_col_block_x;
  Gtk::TreeModelColumn<unsigned int> m_col_block_y;
  Gtk::TreeModelColumn<unsigned int> m_col_block_z;
  Gtk::TreeModelColumn<unsigned int> m_col_thread_x;
  Gtk::TreeModelColumn<unsigned int> m_col_thread_y;
  Gtk::TreeModelColumn<unsigned int> m_col_thread_z;
  Gtk::TreeModelColumn<std::string> m_col_value;
};

ThreadOutputColumns threadOutputColumns;

Glib::RefPtr<Gtk::ListStore> threadOutputTreeModel;

void set_text_entry(std::string id, std::string text)
{
  Gtk::Entry* pEntry;
  refBuilder->get_widget(id, pEntry);
  if (pEntry)
    pEntry->set_text(text);
}

void set_text_views(std::string cuFileName) {
  Gtk::TextView* pDebugTextView;
  refBuilder->get_widget(DEBUG_TEXTVIEW_ID, pDebugTextView);
  Gtk::TextView* pOptimizeConfigTextView;
  refBuilder->get_widget(OPT_CONFIG_TEXTVIEW_ID, pOptimizeConfigTextView);
  if (pDebugTextView && pOptimizeConfigTextView) {
    std::ifstream cuFile(cuFileName);
    std::stringstream fileContents;
    fileContents << cuFile.rdbuf();
    pDebugTextView->get_buffer()->set_text(fileContents.str());
    pOptimizeConfigTextView->get_buffer()->set_text(fileContents.str());
    cuFile.close();
  }
}

void set_entry_from_textview(std::string buttonId, std::string textViewId, std::string entryId) {
  Gtk::Button* setValueButton;
  refBuilder->get_widget(buttonId, setValueButton);
  setValueButton->signal_clicked().connect([textViewId, entryId] () {
    Gtk::TextView* textView;
    refBuilder->get_widget(textViewId, textView);
    // get selected text in text view
    Gtk::TextBuffer::iterator range_start;
    Gtk::TextBuffer::iterator range_end;
    std::string selected;
    if (textView->get_buffer()->get_selection_bounds(range_start, range_end)) {
      selected = textView->get_buffer()->get_text(range_start, range_end, true);
    } else {
      selected = "";
    }
    // set entry to selected text
    Gtk::Entry* entry;
    refBuilder->get_widget(entryId, entry);
    entry->set_text(selected);
  });
}

void save_config() {
  Gtk::Entry* makeEntry;
  refBuilder->get_widget(CONFIG_MAKE_ENTRY_ID, makeEntry);
  Gtk::Entry* runEntry;
  refBuilder->get_widget(CONFIG_RUN_ENTRY_ID, runEntry);
  Gtk::Entry* cuEntry;
  refBuilder->get_widget(CONFIG_CU_ENTRY_ID, cuEntry);

  // save to text file
  std::ofstream configWriteFile(CONFIG_FILENAME);
  if (configWriteFile) {
    configWriteFile << makeEntry->get_text() + "\n" + runEntry->get_text() + "\n" + cuEntry->get_text();
    configWriteFile.close();
  }

  // populate text views with file contents
  set_text_views(cuEntry->get_text());
}

void get_config_from_file(std::string *makeConfig, std::string *runConfig, std::string *cuConfig) {
  // Create a text string, which is used to output the text file
  std::string configText;

  // Read from the text file
  std::ifstream configReadFile(CONFIG_FILENAME);

  getline(configReadFile, configText);
  *makeConfig = configText;

  getline(configReadFile, configText);
  *runConfig = configText;

  getline(configReadFile, configText);
  *cuConfig = configText;

  // Close the file
  configReadFile.close();
}

void init_config_from_file() {
  std::string makeConfig;
  std::string runConfig;
  std::string cuConfig;

  get_config_from_file(&makeConfig, &runConfig, &cuConfig);

  set_text_entry(CONFIG_MAKE_ENTRY_ID, makeConfig);
  set_text_entry(CONFIG_RUN_ENTRY_ID, runConfig);
  set_text_entry(CONFIG_CU_ENTRY_ID, cuConfig);
  save_config();
}

// std::string get_selected_text(Gtk::TextView *textView) {
//   Gtk::TextBuffer::iterator range_start;
//   Gtk::TextBuffer::iterator range_end;
//   if (textView->get_buffer()->get_selection_bounds(range_start, range_end)) {
//     return textView->get_buffer()->get_text(range_start, range_end, true);
//   } else {
//     return "";
//   }
// }

void get_thread_output() {
  // gui input => bash input
  std::string makeConfig;
  std::string runConfig;
  std::string cuConfig;

  get_config_from_file(&makeConfig, &runConfig, &cuConfig);

  Gtk::Entry* threadOutputValueEntry;
  refBuilder->get_widget(THREAD_OUTPUT_VALUE_ENTRY_ID, threadOutputValueEntry);
  std::string valueInput = threadOutputValueEntry->get_text();

  Gtk::Entry* threadOutputValueTypeEntry;
  refBuilder->get_widget(THREAD_OUTPUT_VALUE_TYPE_ENTRY_ID, threadOutputValueTypeEntry);
  std::string typeInput = threadOutputValueTypeEntry->get_text();

  Gtk::Entry* threadOutputLineEntry;
  refBuilder->get_widget(THREAD_OUTPUT_LINE_ENTRY_ID, threadOutputLineEntry);
  std::string lineInput = threadOutputLineEntry->get_text();

  std::cout << "./threadOutput.sh -m " + makeConfig + " -r " + runConfig + " -c " + cuConfig + " -v " + valueInput + " -t " + typeInput + " -l " + lineInput << std::endl;
  
  // TODO: run ./threadOutput.sh -m makeConfig -r runConfig -c cuConfig -v valueInput -t typeInput -l lineInput

  // bash output => gui output
  threadOutputTreeModel = Gtk::ListStore::create(threadOutputColumns);
  Gtk::TreeView* treeView;
  refBuilder->get_widget(THREAD_OUTPUT_TREEVIEW_ID, treeView);
  treeView->set_model(threadOutputTreeModel);

  std::string line;
  std::ifstream threadOutputFile("output/threadOutput.txt");

  while (threadOutputFile) {
    getline(threadOutputFile, line);

    std::istringstream lineStringStream(line);
    std::string token;
    auto row = *(threadOutputTreeModel->append());
    getline(lineStringStream, token, ' ');
    row[threadOutputColumns.m_col_block_x] = stoi(token);
    getline(lineStringStream, token, ' ');
    row[threadOutputColumns.m_col_block_y] = stoi(token);
    getline(lineStringStream, token, ' ');
    row[threadOutputColumns.m_col_block_z] = stoi(token);
    getline(lineStringStream, token, ' ');
    row[threadOutputColumns.m_col_thread_x] = stoi(token);
    getline(lineStringStream, token, ' ');
    row[threadOutputColumns.m_col_thread_y] = stoi(token);
    getline(lineStringStream, token, ' ');
    row[threadOutputColumns.m_col_thread_z] = stoi(token);
    getline(lineStringStream, token, ' ');
    row[threadOutputColumns.m_col_value] = token;
  }

  treeView->append_column("Block X", threadOutputColumns.m_col_block_x);
  treeView->append_column("Block Y", threadOutputColumns.m_col_block_y);
  treeView->append_column("Block Z", threadOutputColumns.m_col_block_z);
  treeView->append_column("Thread X", threadOutputColumns.m_col_thread_x);
  treeView->append_column("Thread Y", threadOutputColumns.m_col_thread_y);
  treeView->append_column("Thread Z", threadOutputColumns.m_col_thread_z);
  treeView->append_column("Value", threadOutputColumns.m_col_value);
}

void get_thread_overwrite() {
  // gui input => bash input
  std::string makeConfig;
  std::string runConfig;
  std::string cuConfig;

  get_config_from_file(&makeConfig, &runConfig, &cuConfig);

  Gtk::Entry* threadOverwriteValueEntry;
  refBuilder->get_widget(THREAD_OVERWRITE_VALUE_ENTRY_ID, threadOverwriteValueEntry);
  std::string valueInput = threadOverwriteValueEntry->get_text();

  Gtk::Entry* threadOverwriteLineEntry;
  refBuilder->get_widget(THREAD_OVERWRITE_LINE_ENTRY_ID, threadOverwriteLineEntry);
  std::string lineInput = threadOverwriteLineEntry->get_text();

  std::cout << "./threadOverwrite.sh -m " + makeConfig + " -r " + runConfig + " -c " + cuConfig + " -v " + valueInput + " -l " + lineInput << std::endl;
}

void optimize_config() {
  // gui input => bash input
  std::string makeConfig;
  std::string runConfig;
  std::string cuConfig;

  get_config_from_file(&makeConfig, &runConfig, &cuConfig);

  std::string blockDimNames = "";
  std::string gridDimNames = "";
  std::string configValues = "";
  std::vector<std::vector<int>> ranges;

  for (int i = 0; i < 6; i++) {
    std::string iStr = std::to_string(i);
    Gtk::CheckButton* checkBox;
    refBuilder->get_widget(OPT_CONFIG_DIM_CHECKBOX_ID_PREFIX + iStr, checkBox);
    Gtk::Entry* entry;
    refBuilder->get_widget(OPT_CONFIG_DIM_ENTRY_ID_PREFIX + iStr, entry);
    Gtk::Entry* startEntry;
    refBuilder->get_widget(OPT_CONFIG_DIM_START_ENTRY_ID_PREFIX + iStr, startEntry);
    Gtk::Entry* stopEntry;
    refBuilder->get_widget(OPT_CONFIG_DIM_STOP_ENTRY_ID_PREFIX + iStr, stopEntry);
    Gtk::Entry* stepEntry;
    refBuilder->get_widget(OPT_CONFIG_DIM_STEP_ENTRY_ID_PREFIX + iStr, stepEntry);
    // if (checkBox->get_active()) {
    std::vector<int> range;
    if (checkBox->get_active()) {
      if (i < 3) {
        blockDimNames += entry->get_text() + ",";
      } else {
        gridDimNames += entry->get_text() + ",";
      }
      int start = stoi(std::string(startEntry->get_text()));
      int stop = stoi(std::string(stopEntry->get_text()));
      int step = stoi(std::string(stepEntry->get_text()));
      for (int dim = start; dim <= stop; dim *= step) {
        range.push_back(dim);
      }
    } else {
      if (i < 3) {
        blockDimNames += ",";
      } else {
        gridDimNames += ",";
      }
      
      range.push_back(0); // add dummy value (gets ignored by bash script)
    }
    ranges.push_back(range);
  }

  for (int gridX : ranges[0]) {
    for (int gridY : ranges[1]) {
      for (int gridZ : ranges[2]) {
        for (int blockX : ranges[3]) {
          for (int blockY : ranges[4]) {
            for (int blockZ : ranges[5]) {
              configValues += std::to_string(gridX) + " " + std::to_string(gridY) + " " + std::to_string(gridZ) + " " + std::to_string(blockX) + " " + std::to_string(blockY) + " " + std::to_string(blockZ) + ",";
            }
          }
        }
      }
    }
  }

  // remove last comma
  configValues.pop_back();

  std::cout << blockDimNames << std::endl;
  std::cout << gridDimNames << std::endl;
  std::cout << configValues << std::endl;
}

void init_config_page() {
  Gtk::Button* pConfigSaveButton;
  refBuilder->get_widget(CONFIG_SAVE_BUTTON_ID, pConfigSaveButton);
  if (pConfigSaveButton) {
    pConfigSaveButton->signal_clicked().connect([] () { save_config(); });
  }

  init_config_from_file();
}

void init_debug_page() {
  // TODO: set thread output line number to cursor position
  
  // set thread output value entry to cursor selection
  set_entry_from_textview(SET_THREAD_OUTPUT_VALUE_BUTTON_ID, DEBUG_TEXTVIEW_ID, THREAD_OUTPUT_VALUE_ENTRY_ID);

  // thread output button
  Gtk::Button* pThreadOutputButton;
  refBuilder->get_widget(THREAD_OUTPUT_SUBMIT_BUTTON_ID, pThreadOutputButton);
  pThreadOutputButton->signal_clicked().connect([] () { 
    system("src/scripts/threads.sh");
    get_thread_output();
  });

  // TODO: set thread overwrite line number to cursor position

  // set thread overwrite value entry to cursor selection
  set_entry_from_textview(SET_THREAD_OVERWRITE_VALUE_BUTTON_ID, DEBUG_TEXTVIEW_ID, THREAD_OVERWRITE_VALUE_ENTRY_ID);

  // thread overwrite button
  Gtk::Button* pThreadOverwriteButton;
  refBuilder->get_widget(THREAD_OVERWRITE_SUBMIT_BUTTON_ID, pThreadOverwriteButton);
  pThreadOverwriteButton->signal_clicked().connect([] () { 
    get_thread_overwrite();
  });
}

void init_optimize_page() {
  for (int i = 0; i < 6; i++) {
    std::string iStr = std::to_string(i);
    // set thread output value entry to cursor selection
    set_entry_from_textview(SET_OPT_CONFIG_DIM_BUTTON_ID_PREFIX + iStr, OPT_CONFIG_TEXTVIEW_ID, OPT_CONFIG_DIM_ENTRY_ID_PREFIX + iStr);
  }
}

void on_app_activate()
{
  std::cerr << "Activating" << std::endl;
  // Load the GtkBuilder file and instantiate its widgets:
  // refBuilder = Gtk::Builder::create();
  try
  {
    // refBuilder->add_from_file("src/gui4.glade");
    refBuilder = Gtk::Builder::create_from_file("src/gui.glade");
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
  Gtk::Window* pWindow;
  refBuilder->get_widget("window", pWindow);
  if (!pWindow)
  {
    std::cerr << "Could not get the window" << std::endl;
    return;
  }

  init_config_page();

  init_debug_page();

  init_optimize_page();

  // It's not possible to delete widgets after app->run() has returned.
  // Delete the window with its child widgets before app->run() returns.
  pWindow->signal_hide().connect([&] () { delete pWindow; });

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