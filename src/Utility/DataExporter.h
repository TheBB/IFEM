// $Id$

#pragma once

#include <map>
#include <string>
#include <vector>


class DataWriter;
class SIMparameters;

class DataExporter
{
 public:
  enum FieldType {
    VECTOR,
    SIM
  };

  enum Results {
    PRIMARY=0,
    SECONDARY=1,
    NORMS=2
  };

  struct FileEntry {
    std::string description;
    FieldType field;
    int results;
    void* data;
    void* data2;
  };

  //! \brief Default constructor.
  //! \param[in] dynWriters If \e true, delete the writers on destruction.
  //! \param[in] ndump Interval between dumps
  //! \param[in] order The temporal order of simulations (always dumps order solutions in a row)
  DataExporter(bool dynWriters = false, int ndump=1, int order=1) :
    m_delete(dynWriters), m_level(-1), m_ndump(ndump), m_order(order)
  {
  }

  //! \brief The destructor deletes the writers if \a dynWriters was \e true.
  ~DataExporter();

  //! \brief Registers an entry for storage.
  //! param[in] name Name of entry
  //! param[in] description Description of entry
  //! param[in] field Type of entry
  //! param[in] results Which results to store
  //! the time level to use for SIM
  bool registerField(const std::string& name,
		     const std::string& description,
		     FieldType field, int results=PRIMARY);

  bool registerWriter(DataWriter* writer);

  bool setFieldValue(const std::string& name, void* data, void* data2=NULL);

  bool dumpTimeLevel(SIMparameters* tp=NULL, bool geometryUpdated=false);

  //! \brief Loads last time level with first registered writer by default.
  //! param[in] level Time level to load, defaults to last time level
  //! param[in] info The datawriter to read the info from (e.g. the XML writer)
  //! param[in] input The datawriter to read the data from (e.g. the HDF5 writer)
  bool loadTimeLevel(int level=-1, DataWriter* info=NULL, DataWriter* input=NULL);
  int getTimeLevel();
protected:
  int getWritersTimeLevel() const;

  std::map<std::string,FileEntry> m_entry;
  std::vector<DataWriter*> m_writers;
  bool m_delete;
  int m_level;
  int m_ndump;
  int m_order;
};


typedef std::pair<std::string,DataExporter::FileEntry> DataEntry;

class DataWriter
{
protected:
  DataWriter(const std::string& name);

public:
  virtual ~DataWriter() {}

  virtual int getLastTimeLevel() = 0;

  virtual void openFile(int level) = 0;
  virtual void closeFile(int level, bool force=false) = 0;

  virtual void writeVector(int level, const DataEntry& entry) = 0;
  virtual bool readVector(int level, const DataEntry& entry) = 0;
  virtual void writeSIM(int level, const DataEntry& entry,
                        bool geometryUpdated) = 0;
  virtual bool readSIM(int level, const DataEntry& entry) = 0;

  virtual bool writeTimeInfo(int level, int order, int interval,
                             SIMparameters& tp) = 0;

protected:
  std::string m_name; //!< File name

  int m_size; //!< Number of MPI nodes (processors)
  int m_rank; //!< MPI rank (processor ID)
};
