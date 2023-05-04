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
#define THREAD_OVERWRITE_VALUE_TYPE_ENTRY_ID "thread_overwrite_value_type_entry"
#define THREAD_OVERWRITE_IS_ARRAY_CHECKBOX_ID "thread_overwrite_is_array_checkbox"
#define THREAD_OVERWRITE_SUBMIT_BUTTON_ID "thread_overwrite_button"
#define THREAD_OVERWRITE_TREEVIEW_ID "thread_overwrite_treeview"

#define OPT_CONFIG_TEXTVIEW_ID "opt_config_textview"

#define OPT_CONFIG_DIM_CHECKBOX_ID_PREFIX "opt_config_dim_checkbox"
#define OPT_CONFIG_DIM_ENTRY_ID_PREFIX "opt_config_dim_entry"
#define SET_OPT_CONFIG_DIM_BUTTON_ID_PREFIX "set_opt_config_dim_button"
#define OPT_CONFIG_DIM_START_ENTRY_ID_PREFIX "opt_config_dim_start_entry"
#define OPT_CONFIG_DIM_STOP_ENTRY_ID_PREFIX "opt_config_dim_stop_entry"
#define OPT_CONFIG_DIM_STEP_ENTRY_ID_PREFIX "opt_config_dim_step_entry"

#define OPT_CONFIG_TEST_CONFIG_BUTTON_ID "opt_config_test_config_button"
#define OPT_CONFIG_TREEVIEW_ID "opt_config_treeview"

#define TIME_BOTTLENECK_PIECHART_BUTTON_ID "time_bottleneck_piechart_button"
#define TIME_BOTTLENECK_PIECHART_IMAGE_ID "time_bottleneck_piechart_image"

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

class ThreadOverwriteColumns : public Gtk::TreeModel::ColumnRecord
{
public:

  ThreadOverwriteColumns() {
    add(m_col_address);
    add(m_col_block_x);
    add(m_col_block_y);
    add(m_col_block_z);
    add(m_col_thread_x);
    add(m_col_thread_y);
    add(m_col_thread_z);
    add(m_col_index);
    add(m_col_value);
  }

  Gtk::TreeModelColumn<std::string> m_col_address;
  Gtk::TreeModelColumn<std::string> m_col_block_x;
  Gtk::TreeModelColumn<std::string> m_col_block_y;
  Gtk::TreeModelColumn<std::string> m_col_block_z;
  Gtk::TreeModelColumn<std::string> m_col_thread_x;
  Gtk::TreeModelColumn<std::string> m_col_thread_y;
  Gtk::TreeModelColumn<std::string> m_col_thread_z;
  Gtk::TreeModelColumn<std::string> m_col_index;
  Gtk::TreeModelColumn<std::string> m_col_value;
};

ThreadOverwriteColumns threadOverwriteColumns;
Glib::RefPtr<Gtk::TreeStore> threadOverwriteTreeModel;

class OptimizeConfigColumns : public Gtk::TreeModel::ColumnRecord
{
public:

  OptimizeConfigColumns() {
    add(m_col_grid_x);
    add(m_col_grid_y);
    add(m_col_grid_z);
    add(m_col_block_x);
    add(m_col_block_y);
    add(m_col_block_z);
    add(m_col_time);
  }

  Gtk::TreeModelColumn<unsigned int> m_col_grid_x;
  Gtk::TreeModelColumn<unsigned int> m_col_grid_y;
  Gtk::TreeModelColumn<unsigned int> m_col_grid_z;
  Gtk::TreeModelColumn<unsigned int> m_col_block_x;
  Gtk::TreeModelColumn<unsigned int> m_col_block_y;
  Gtk::TreeModelColumn<unsigned int> m_col_block_z;
  Gtk::TreeModelColumn<unsigned int> m_col_time;
};

OptimizeConfigColumns optimizeConfigColumns;
Glib::RefPtr<Gtk::ListStore> optimizeConfigTreeModel;

struct OptimizeConfigResult {
  int gridX;
  int gridY;
  int gridZ;
  int blockX;
  int blockY;
  int blockZ;
  int time;

  OptimizeConfigResult(int gX, int gY, int gZ, int bX, int bY, int bZ, int t) : gridX(gX), gridY(gY), gridZ(gZ), blockX(bX), blockY(bY), blockZ(bZ), time(t) {}
};

// Sorts results in increasing time order (fastest config to slowest config)
struct LessThanKeyOptimizeConfigResult {
  inline bool operator() (const OptimizeConfigResult& struct1, const OptimizeConfigResult& struct2)
  {
      return (struct1.time < struct2.time);
  }
};

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

void set_entry_from_textview_linenum(std::string buttonId, std::string textViewId, std::string entryId) {
  Gtk::Button* setValueButton;
  refBuilder->get_widget(buttonId, setValueButton);
  setValueButton->signal_clicked().connect([textViewId, entryId] () { 
    Gtk::TextView* textView;
    refBuilder->get_widget(textViewId, textView);

    // get all the text before the cursor
    Gtk::TextBuffer::iterator range_start = textView->get_buffer()->begin();
    Gtk::TextBuffer::iterator range_end;
    Gtk::TextBuffer::iterator range_end2;
    textView->get_buffer()->get_selection_bounds(range_end, range_end2);
    std::stringstream ss(textView->get_buffer()->get_text(range_start, range_end));

    // get number of newlines
    std::string line = "";
    int lineNum = 0;
    while (getline(ss, line)) {
      lineNum++;
    }

    // account for cursor at start of a line
    if (line == "") {
      lineNum++;
    }

    // set entry to line num
    Gtk::Entry* entry;
    refBuilder->get_widget(entryId, entry);
    entry->set_text(std::to_string(lineNum));
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

  Gtk::Entry* threadOverwriteValueTypeEntry;
  refBuilder->get_widget(THREAD_OVERWRITE_VALUE_TYPE_ENTRY_ID, threadOverwriteValueTypeEntry);
  std::string valueTypeInput = threadOverwriteValueTypeEntry->get_text();

  Gtk::CheckButton* threadOverwriteCheckButton;
  refBuilder->get_widget(THREAD_OVERWRITE_IS_ARRAY_CHECKBOX_ID, threadOverwriteCheckButton);
  std::string isArrayInput = (threadOverwriteCheckButton->get_active()) ? "y" : "n";

  std::cout << "./threadOverwrite.sh -m " + makeConfig + " -r " + runConfig + " -c " + cuConfig + " -v " + valueInput + " -t " + valueTypeInput + " -l " + lineInput + " -a " + isArrayInput << std::endl;

  // bash output => gui output
  threadOverwriteTreeModel = Gtk::TreeStore::create(threadOverwriteColumns);
  Gtk::TreeView* treeView;
  refBuilder->get_widget(THREAD_OVERWRITE_TREEVIEW_ID, treeView);
  treeView->set_model(threadOverwriteTreeModel);

  std::string line;
  std::ifstream threadOverwriteFile("output/threadOverwrite.txt");

  while (threadOverwriteFile) {
    getline(threadOverwriteFile, line);

    // split line by spaces
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (getline(ss, token, ' ')) {
      tokens.push_back(token);
    }

    // address is the first token
    auto addressRow = *(threadOverwriteTreeModel->append());
    std::string address = tokens[0];
    addressRow[threadOverwriteColumns.m_col_address] = address;

    // rest of the tokens correspond to the threads that wrote to the address
    for (int i = 1; i < tokens.size(); i++) {
      std::stringstream ssTuple(tokens[i]);
      std::vector<std::string> tuple;
      std::string tokenTuple;
      while (getline(ssTuple, tokenTuple, ',')) {
        tuple.push_back(tokenTuple);
      }
      auto row = *(threadOverwriteTreeModel->append(addressRow.children()));
      row[threadOverwriteColumns.m_col_address] = address;
      row[threadOverwriteColumns.m_col_block_x] = tuple[0];
      row[threadOverwriteColumns.m_col_block_y] = tuple[1];
      row[threadOverwriteColumns.m_col_block_z] = tuple[2];
      row[threadOverwriteColumns.m_col_thread_x] = tuple[3];
      row[threadOverwriteColumns.m_col_thread_y] = tuple[4];
      row[threadOverwriteColumns.m_col_thread_z] = tuple[5];
      row[threadOverwriteColumns.m_col_index] = tuple[6];
      row[threadOverwriteColumns.m_col_value] = tuple[7];
    }
  }

  treeView->append_column("Address", threadOverwriteColumns.m_col_address);
  treeView->append_column("Block X", threadOverwriteColumns.m_col_block_x);
  treeView->append_column("Block Y", threadOverwriteColumns.m_col_block_y);
  treeView->append_column("Block Z", threadOverwriteColumns.m_col_block_z);
  treeView->append_column("Thread X", threadOverwriteColumns.m_col_thread_x);
  treeView->append_column("Thread Y", threadOverwriteColumns.m_col_thread_y);
  treeView->append_column("Thread Z", threadOverwriteColumns.m_col_thread_z);
  treeView->append_column("Index", threadOverwriteColumns.m_col_index);
  treeView->append_column("Value", threadOverwriteColumns.m_col_value);
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

  int numConfigs = 0;
  for (int gridX : ranges[0]) {
    for (int gridY : ranges[1]) {
      for (int gridZ : ranges[2]) {
        for (int blockX : ranges[3]) {
          for (int blockY : ranges[4]) {
            for (int blockZ : ranges[5]) {
              configValues += std::to_string(gridX) + " " + std::to_string(gridY) + " " + std::to_string(gridZ) + " " + std::to_string(blockX) + " " + std::to_string(blockY) + " " + std::to_string(blockZ) + ",";
              numConfigs++;
            }
          }
        }
      }
    }
  }

  // remove last comma
  configValues.pop_back();

  // TODO: run script
  numConfigs = 3; // REMOVE THIS (this is for dummy data)
  std::cout << "./optimizeConfig.sh -m " + makeConfig + " -r " + runConfig + " -b " + blockDimNames + " -g " + gridDimNames + " -v " + configValues << std::endl;

  // display output
  // bash output => gui output
  optimizeConfigTreeModel = Gtk::ListStore::create(optimizeConfigColumns);
  Gtk::TreeView* treeView;
  refBuilder->get_widget(OPT_CONFIG_TREEVIEW_ID, treeView);
  treeView->set_model(optimizeConfigTreeModel);

  std::string line;
  std::ifstream optimizeConfigFile("output/optimizeConfig.txt");
  std::vector<OptimizeConfigResult> results;
  for (int config = 0; config < numConfigs; config++) {
    // get first line
    getline(optimizeConfigFile, line);

    std::istringstream lineStringStream(line);
    std::string token;
    float time = 0.0f;
    getline(lineStringStream, token, ' ');
    int gridX = stoi(token);
    getline(lineStringStream, token, ' ');
    int gridY = stoi(token);
    getline(lineStringStream, token, ' ');
    int gridZ = stoi(token);
    getline(lineStringStream, token, ' ');
    int blockX = stoi(token);
    getline(lineStringStream, token, ' ');
    int blockY = stoi(token);
    getline(lineStringStream, token, ' ');
    int blockZ = stoi(token);
    getline(lineStringStream, token, ' ');
    time += stof(token);

    // get next 2 lines (same config, but just access the time)
    for (int i = 0; i < 2; i++) {
      getline(optimizeConfigFile, line);
      // ignore first 6 tokens
      for (int j = 0; j < 6; j++) {
        getline(lineStringStream, token, ' ');
      }
      // get elapsed time (7th token)
      getline(lineStringStream, token, ' ');
      time += stof(token);
    }

    // calculate average time in microseconds
    int avgtime = time / 3 * 1000000;
    // row[optimizeConfigColumns.m_col_time] = avgtime;
    
    results.push_back(OptimizeConfigResult(gridX, gridY, gridZ, blockX, blockY, blockZ, avgtime));
  }

  std::sort(results.begin(), results.end(), LessThanKeyOptimizeConfigResult());

  // populate tree with rows
  for (int i = 0; i < results.size(); i++) {
    auto row = *(optimizeConfigTreeModel->append());
    row[optimizeConfigColumns.m_col_grid_x] = results[i].gridX;
    row[optimizeConfigColumns.m_col_grid_y] = results[i].gridY;
    row[optimizeConfigColumns.m_col_grid_z] = results[i].gridZ;
    row[optimizeConfigColumns.m_col_block_x] = results[i].blockX;
    row[optimizeConfigColumns.m_col_block_y] = results[i].blockY;
    row[optimizeConfigColumns.m_col_block_z] = results[i].blockZ;
    row[optimizeConfigColumns.m_col_time] = results[i].time;
  }

  treeView->append_column("Grid X", optimizeConfigColumns.m_col_grid_x);
  treeView->append_column("Grid Y", optimizeConfigColumns.m_col_grid_y);
  treeView->append_column("Grid Z", optimizeConfigColumns.m_col_grid_z);
  treeView->append_column("Block X", optimizeConfigColumns.m_col_block_x);
  treeView->append_column("Block Y", optimizeConfigColumns.m_col_block_y);
  treeView->append_column("Block Z", optimizeConfigColumns.m_col_block_z);
  treeView->append_column("Time (microseconds)", optimizeConfigColumns.m_col_time);

}

void get_time_bottleneck_piechart() {
  // TODO: run script here
  Gtk::Image* piechartImage;
  refBuilder->get_widget(TIME_BOTTLENECK_PIECHART_IMAGE_ID, piechartImage);

  // display pie chart
  Glib::RefPtr<Gdk::Pixbuf> piechart = Gdk::Pixbuf::create_from_file("output/gprofTime.png", 900, 900);
  // Glib::RefPtr<Gdk::Pixbuf> scaled = piechart->scale_simple(400, 500, Gdk::InterpType.BILINEAR);
  piechartImage->set(piechart);
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
  // Gtk::Button* pThreadOutputLineButton;
  // refBuilder->get_widget(SET_THREAD_OUTPUT_LINE_BUTTON_ID, pThreadOutputLineButton);
  // pThreadOutputLineButton->signal_clicked().connect([] () { 
  //   Gtk::TextView* textView;
  //   refBuilder->get_widget(DEBUG_TEXTVIEW_ID, textView);
  //   // Gtk::TextBuffer::iterator range_start = textView->get_buffer()->get_iter_at_line(0);
  //   Gtk::TextBuffer::iterator range_start = textView->get_buffer()->begin();
  //   Gtk::TextBuffer::iterator range_end;
  //   Gtk::TextBuffer::iterator range_end2;
  //   textView->get_buffer()->get_selection_bounds(range_end, range_end2);
  //   std::cout << textView->get_buffer()->get_text(range_start, range_end) << std::endl;

  //   std::stringstream ss(textView->get_buffer()->get_text(range_start, range_end));
  //   std::string line = "";
  //   int numLines = 0;
  //   while (getline(ss, line)) {
  //     numLines++;
  //   }
  //   // account for cursor at start of a line
  //   if (line == "") {
  //     numLines++;
  //   }
  //   std::cout << numLines << std::endl;

  // });
  set_entry_from_textview_linenum(SET_THREAD_OUTPUT_LINE_BUTTON_ID, DEBUG_TEXTVIEW_ID, THREAD_OUTPUT_LINE_ENTRY_ID);
  
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
  set_entry_from_textview_linenum(SET_THREAD_OVERWRITE_LINE_BUTTON_ID, DEBUG_TEXTVIEW_ID, THREAD_OVERWRITE_LINE_ENTRY_ID);

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

  // test config button
  Gtk::Button* testConfigButton;
  refBuilder->get_widget(OPT_CONFIG_TEST_CONFIG_BUTTON_ID, testConfigButton);
  testConfigButton->signal_clicked().connect([] () { 
    optimize_config();
  });

  // time bottleneck pie chart
  Gtk::Button* pieChartButton;
  refBuilder->get_widget(TIME_BOTTLENECK_PIECHART_BUTTON_ID, pieChartButton);
  pieChartButton->signal_clicked().connect([] () { 
    get_time_bottleneck_piechart();
  });
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