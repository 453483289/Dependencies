// ClrPhlib.h

#pragma once

#include <UnmanagedPh.h>


#using <System.dll>

namespace System {

    using namespace Collections::Generic;

    namespace ClrPh {

		public ref class ApiSetTarget : List<String^>
		{};

		public ref class ApiSetSchema : Dictionary<String^, ApiSetTarget^>
		{};

        public ref class Phlib {
        public:

            // Imitialize Process Hacker's phlib internal data
            // Must be called before any other API (kinda like OleInitialize).
            static bool InitializePhLib();

            // Return the list of knwown dll for this system
            static List<String^>^ GetKnownDlls(_In_ bool Wow64Dlls);

            static List<String^>^ KnownDll64List;
            static List<String^>^ KnownDll32List;

			
			// Return the Api Set schema:
			// NB: Api set resolution rely on hash buckets who 
			// can contains more entries than this schema.
			static ApiSetSchema^ GetApiSetSchema();

			// Resolve an api set dll. Return empty string if there is no host dll.
			// static String^ ResolveApiSet(_In_ String^ ApiSetLibraryName);
        };



        public ref struct PeImport {
            Int16 Hint;
            Int16 Ordinal;
            String ^ Name;
            String ^ ModuleName;
            Boolean ImportByOrdinal;
            Boolean DelayImport;

            PeImport(const PPH_MAPPED_IMAGE_IMPORT_DLL importDll, size_t Index);
            PeImport(const PeImport ^ other);
            ~PeImport();

        };

        public ref struct PeImportDll {
        public:
            Int64 Flags;
            String ^Name;
            Int64 NumberOfEntries;

            List<PeImport^>^ ImportList;

            PeImportDll(const PPH_MAPPED_IMAGE_IMPORTS &PvMappedImports, size_t ImportDllIndex);
            PeImportDll(const PeImportDll ^ other);
            ~PeImportDll();
        protected:
            !PeImportDll();

        private:
            PPH_MAPPED_IMAGE_IMPORT_DLL ImportDll;
        };

        public ref struct PeExport {
            Int16 Ordinal;
            String ^  Name; // may be NULL.
            Boolean ExportByOrdinal;
            Int64   VirtualAddress;
            String ^  ForwardedName;

            PeExport(const UnmanagedPE &refPe, size_t Index);
            PeExport(const PeExport ^ other);
            ~PeExport();

        };

        public ref struct PeProperties {
            Int16 Machine;
            DateTime ^ Time;
            Int16 Magic;

            IntPtr ImageBase;
            Int32  SizeOfImage;
            IntPtr EntryPoint;


            Int32 Checksum;
            Boolean CorrectChecksum;

            Int16 Subsystem;
            Tuple<Int16, Int16> ^SubsystemVersion;

            Int16 Characteristics;
            Int16 DllCharacteristics;

			UInt64 FileSize;
        };



        public ref class PE
        {
        public:
            PE(_In_ String^ Filepath);

            ~PE();

            // Check if the PE is 32-bit
            bool IsWow64Dll();

            // Return the list of functions exported by the PE
            List<PeExport ^>^ GetExports();

            // Return the list of functions imported by the PE, bundled by Dll name
            List<PeImportDll ^>^ GetImports();

            // Retrieve the manifest embedded within the PE
            // Return an empty string if there is none.
            String^ GetManifest();

            PeProperties ^Properties;
            Boolean LoadSuccessful;
            String^ Filepath;

        protected:
            // Deallocate the native object on the finalizer just in case no destructor is called  
            !PE() {
                delete m_Impl;
            }

            void InitProperties();

        private:
            UnmanagedPE * m_Impl;
        };

        public ref class PhSymbolProvider
        {
        public:
            PhSymbolProvider();
            ~PhSymbolProvider();
            !PhSymbolProvider();

            String^ UndecorateName(_In_ String ^DecoratedName);

        private:
            UnmanagedSymPrv *m_Impl;

        };
    }

}