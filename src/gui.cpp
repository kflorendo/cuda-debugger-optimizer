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

#define OPT_CONFIG_GRAPH_ALL_IMAGE_ID "config_graph_all_image"
#define OPT_CONFIG_GRAPH_GRIDX_IMAGE_ID "config_graph_gridx_image"
#define OPT_CONFIG_GRAPH_GRIDY_IMAGE_ID "config_graph_gridy_image"
#define OPT_CONFIG_GRAPH_GRIDZ_IMAGE_ID "config_graph_gridz_image"
#define OPT_CONFIG_GRAPH_BLOCKX_IMAGE_ID "config_graph_blockx_image"
#define OPT_CONFIG_GRAPH_BLOCKY_IMAGE_ID "config_graph_blocky_image"
#define OPT_CONFIG_GRAPH_BLOCKZ_IMAGE_ID "config_graph_blockz_image"

#define TIME_BOTTLENECK_PIECHART_BUTTON_ID "time_bottleneck_piechart_button"
#define TIME_BOTTLENECK_PIECHART_IMAGE_ID "time_bottleneck_piechart_image"

#define TIME_BOTTLENECK_TIMELINE_BUTTON_ID "time_bottleneck_timeline_button"
#define TIME_BOTTLENECK_GPU_TIMELINE_IMAGE_ID "time_bottleneck_gpu_timeline_image"
#define TIME_BOTTLENECK_API_TIMELINE_IMAGE_ID "time_bottleneck_api_timeline_image"

#define BREAKPOINT_BUTTON_ID "breakpoint_button"
#define BREAKPOINT_LINE_ENTRY_ID "breakpoint_line_entry"
#define SET_BREAKPOINT_LINE_BUTTON_ID "set_breakpoint_line_button"
#define BREAKPOINT_TREEVIEW_ID "breakpoint_treeview"

#define MEMORY_BOTTLENECK_STATIC_MEM_IMAGE_ID "static_mem_image"
#define MEMORY_BOTTLENECK_DYNAMIC_MEM_IMAGE_ID "dynamic_mem_image"
#define MEMORY_BOTTLENECK_MEMORY_SIZE_IMAGE_ID "memory_size_image"
#define MEMORY_BOTTLENECK_THROUGHPUT_IMAGE_ID "throughput_image"

#define MEMORY_BOTTLENECK_BUTTON_ID "memory_bottleneck_button"

#define SPEEDUP_SEQUENTIAL_ENTRY_ID "speedup_sequential_entry_id"
#define SPEEDUP_BUTTON_ID "speedup_button_id"
#define SPEEDUP_RESULT_LABEL_ID "speedup_result_label_id"

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

class BreakpointColumns : public Gtk::TreeModel::ColumnRecord
{
public:

  BreakpointColumns() {
    add(m_col_block_x);
    add(m_col_block_y);
    add(m_col_block_z);
    add(m_col_thread_x);
    add(m_col_thread_y);
    add(m_col_thread_z);
  }

  Gtk::TreeModelColumn<unsigned int> m_col_block_x;
  Gtk::TreeModelColumn<unsigned int> m_col_block_y;
  Gtk::TreeModelColumn<unsigned int> m_col_block_z;
  Gtk::TreeModelColumn<unsigned int> m_col_thread_x;
  Gtk::TreeModelColumn<unsigned int> m_col_thread_y;
  Gtk::TreeModelColumn<unsigned int> m_col_thread_z;
};

BreakpointColumns breakpointColumns;
Glib::RefPtr<Gtk::ListStore> breakpointTreeModel;

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

void get_breakpoint() {
  // gui input => bash input
  std::string makeConfig;
  std::string runConfig;
  std::string cuConfig;

  get_config_from_file(&makeConfig, &runConfig, &cuConfig);

  Gtk::Entry* breakpointLineEntry;
  refBuilder->get_widget(BREAKPOINT_LINE_ENTRY_ID, breakpointLineEntry);
  std::string lineInput = breakpointLineEntry->get_text();

  std::cout << "./breakpoint.sh -m " + makeConfig + " -r " + runConfig + " -c " + cuConfig + " -l " + lineInput << std::endl;

  // bash output => gui output
  breakpointTreeModel = Gtk::ListStore::create(breakpointColumns);
  Gtk::TreeView* treeView;
  refBuilder->get_widget(BREAKPOINT_TREEVIEW_ID, treeView);
  treeView->set_model(breakpointTreeModel);

  std::string line;
  std::ifstream breakpointFile("output/threadbp.txt");

  while (getline(breakpointFile, line)) {
    // split line by spaces
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (getline(ss, token, ' ')) {
      tokens.push_back(token);
    }

    auto row = *(breakpointTreeModel->append());
    row[breakpointColumns.m_col_block_x] = stoi(tokens[0]);
    row[breakpointColumns.m_col_block_y] = stoi(tokens[1]);
    row[breakpointColumns.m_col_block_z] = stoi(tokens[2]);
    row[breakpointColumns.m_col_thread_x] = stoi(tokens[3]);
    row[breakpointColumns.m_col_thread_y] = stoi(tokens[4]);
    row[breakpointColumns.m_col_thread_z] = stoi(tokens[5]);
  }

  treeView->append_column("Block X", breakpointColumns.m_col_block_x);
  treeView->append_column("Block Y", breakpointColumns.m_col_block_y);
  treeView->append_column("Block Z", breakpointColumns.m_col_block_z);
  treeView->append_column("Thread X", breakpointColumns.m_col_thread_x);
  treeView->append_column("Thread Y", breakpointColumns.m_col_thread_y);
  treeView->append_column("Thread Z", breakpointColumns.m_col_thread_z);
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

  // display graphs
  Gtk::Image* graphAllImage;
  refBuilder->get_widget(OPT_CONFIG_GRAPH_ALL_IMAGE_ID, graphAllImage);
  Glib::RefPtr<Gdk::Pixbuf> graphAll = Gdk::Pixbuf::create_from_file("output/all.png", 350, 350);
  graphAllImage->set(graphAll);

  Gtk::Image* graphGridXImage;
  refBuilder->get_widget(OPT_CONFIG_GRAPH_GRIDX_IMAGE_ID, graphGridXImage);
  Glib::RefPtr<Gdk::Pixbuf> graphGridX = Gdk::Pixbuf::create_from_file("output/gridx.png", 350, 350);
  graphGridXImage->set(graphGridX);

  Gtk::Image* graphGridYImage;
  refBuilder->get_widget(OPT_CONFIG_GRAPH_GRIDY_IMAGE_ID, graphGridYImage);
  Glib::RefPtr<Gdk::Pixbuf> graphGridY = Gdk::Pixbuf::create_from_file("output/gridy.png", 350, 350);
  graphGridYImage->set(graphGridY);

  Gtk::Image* graphGridZImage;
  refBuilder->get_widget(OPT_CONFIG_GRAPH_GRIDZ_IMAGE_ID, graphGridZImage);
  Glib::RefPtr<Gdk::Pixbuf> graphGridZ = Gdk::Pixbuf::create_from_file("output/gridz.png", 350, 350);
  graphGridZImage->set(graphGridZ);

  Gtk::Image* graphBlockXImage;
  refBuilder->get_widget(OPT_CONFIG_GRAPH_BLOCKX_IMAGE_ID, graphBlockXImage);
  Glib::RefPtr<Gdk::Pixbuf> graphBlockX = Gdk::Pixbuf::create_from_file("output/blockx.png", 350, 350);
  graphBlockXImage->set(graphBlockX);

  Gtk::Image* graphBlockYImage;
  refBuilder->get_widget(OPT_CONFIG_GRAPH_BLOCKY_IMAGE_ID, graphBlockYImage);
  Glib::RefPtr<Gdk::Pixbuf> graphBlockY = Gdk::Pixbuf::create_from_file("output/blocky.png", 350, 350);
  graphBlockYImage->set(graphBlockY);

  Gtk::Image* graphBlockZImage;
  refBuilder->get_widget(OPT_CONFIG_GRAPH_BLOCKZ_IMAGE_ID, graphBlockZImage);
  Glib::RefPtr<Gdk::Pixbuf> graphBlockZ = Gdk::Pixbuf::create_from_file("output/blockz.png", 350, 350);
  graphBlockZImage->set(graphBlockZ);
}

void get_time_bottleneck_piechart() {
  // TODO: run script here
  Gtk::Image* piechartImage;
  refBuilder->get_widget(TIME_BOTTLENECK_PIECHART_IMAGE_ID, piechartImage);

  // display pie chart
  Glib::RefPtr<Gdk::Pixbuf> piechart = Gdk::Pixbuf::create_from_file("output/gprofTime.png", 900, 900);
  piechartImage->set(piechart);
}

void get_time_bottleneck_timelines() {
  // TODO: run script here

  // display gpu image
  Gtk::Image* gpuTimelineImage;
  refBuilder->get_widget(TIME_BOTTLENECK_GPU_TIMELINE_IMAGE_ID, gpuTimelineImage);

  Glib::RefPtr<Gdk::Pixbuf> gpuTimeline = Gdk::Pixbuf::create_from_file("output/timeBottleneckGpuTimeline.png", 500, 500);
  gpuTimelineImage->set(gpuTimeline);

  // display api image
  Gtk::Image* apiTimelineImage;
  refBuilder->get_widget(TIME_BOTTLENECK_API_TIMELINE_IMAGE_ID, apiTimelineImage);

  Glib::RefPtr<Gdk::Pixbuf> apiTimeline = Gdk::Pixbuf::create_from_file("output/timeBottleneckApiTimeline.png", 500, 500);
  apiTimelineImage->set(apiTimeline);
}

void get_memory_bottleneck() {
  // TODO: run script here

  // display gpu image
  Gtk::Image* timeStaticImage;
  refBuilder->get_widget(MEMORY_BOTTLENECK_STATIC_MEM_IMAGE_ID, timeStaticImage);

  Glib::RefPtr<Gdk::Pixbuf> timeStatic = Gdk::Pixbuf::create_from_file("output/timestat.png", 500, 500);
  timeStaticImage->set(timeStatic);

  Gtk::Image* timeDynamicImage;
  refBuilder->get_widget(MEMORY_BOTTLENECK_DYNAMIC_MEM_IMAGE_ID, timeDynamicImage);

  Glib::RefPtr<Gdk::Pixbuf> timeDynamic = Gdk::Pixbuf::create_from_file("output/timedynam.png", 500, 500);
  timeDynamicImage->set(timeDynamic);

  Gtk::Image* timeSizeImage;
  refBuilder->get_widget(MEMORY_BOTTLENECK_MEMORY_SIZE_IMAGE_ID, timeSizeImage);

  Glib::RefPtr<Gdk::Pixbuf> timeSize = Gdk::Pixbuf::create_from_file("output/timesize.png", 500, 500);
  timeSizeImage->set(timeSize);

  Gtk::Image* timeThroughputImage;
  refBuilder->get_widget(MEMORY_BOTTLENECK_THROUGHPUT_IMAGE_ID, timeThroughputImage);

  Glib::RefPtr<Gdk::Pixbuf> timeThroughput = Gdk::Pixbuf::create_from_file("output/timethru.png", 500, 500);
  timeThroughputImage->set(timeThroughput);
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
  // set thread output line number to cursor position
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

  // set thread overwrite line number to cursor position
  set_entry_from_textview_linenum(SET_THREAD_OVERWRITE_LINE_BUTTON_ID, DEBUG_TEXTVIEW_ID, THREAD_OVERWRITE_LINE_ENTRY_ID);

  // set thread overwrite value entry to cursor selection
  set_entry_from_textview(SET_THREAD_OVERWRITE_VALUE_BUTTON_ID, DEBUG_TEXTVIEW_ID, THREAD_OVERWRITE_VALUE_ENTRY_ID);

  // thread overwrite button
  Gtk::Button* pThreadOverwriteButton;
  refBuilder->get_widget(THREAD_OVERWRITE_SUBMIT_BUTTON_ID, pThreadOverwriteButton);
  pThreadOverwriteButton->signal_clicked().connect([] () { 
    get_thread_overwrite();
  });

  // set breakpoint line number to cursor position
  set_entry_from_textview_linenum(SET_BREAKPOINT_LINE_BUTTON_ID, DEBUG_TEXTVIEW_ID, BREAKPOINT_LINE_ENTRY_ID);

  // breakpoint button
  Gtk::Button* pBreakpointButton;
  refBuilder->get_widget(BREAKPOINT_BUTTON_ID, pBreakpointButton);
  pBreakpointButton->signal_clicked().connect([] () { 
    get_breakpoint();
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

  // timeline button
  Gtk::Button* timelineButton;
  refBuilder->get_widget(TIME_BOTTLENECK_TIMELINE_BUTTON_ID, timelineButton);
  timelineButton->signal_clicked().connect([] () { 
    get_time_bottleneck_timelines();
  });

  // memory bottleneck button
  Gtk::Button* memoryBottleneckButton;
  refBuilder->get_widget(MEMORY_BOTTLENECK_BUTTON_ID, memoryBottleneckButton);
  memoryBottleneckButton->signal_clicked().connect([] () { 
    get_memory_bottleneck();
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