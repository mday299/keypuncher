# Path to the target folder containing DLLs
$targetFolder = "C:\Users\mday39\dev\testDLLs"

# Function to determine DLL architecture using PE Headers
function Get-DllArchitecture {
    param (
        [string]$dllPath
    )

    try {
        $bytes = [System.IO.File]::ReadAllBytes($dllPath)

        # Check the MZ header to validate the file type
        $e_magic = [System.BitConverter]::ToUInt16($bytes, 0)
        if ($e_magic -ne 0x5A4D) { return "Invalid PE file" }

        # Locate the PE header at offset 0x3C
        $peHeaderOffset = [System.BitConverter]::ToUInt32($bytes, 0x3C)
        $peSignature = [System.BitConverter]::ToUInt32($bytes, $peHeaderOffset)
        if ($peSignature -ne 0x4550) { return "Invalid PE signature" }

        # Read the machine type (architecture)
        $machineType = [System.BitConverter]::ToUInt16($bytes, $peHeaderOffset + 4)
        switch ($machineType) {
            0x014C { return "x86" }        # IMAGE_FILE_MACHINE_I386
            0x8664 { return "x64" }        # IMAGE_FILE_MACHINE_AMD64
            0x0200 { return "IA64 (Itanium)" }
            default { return "Unknown" }
        }
    } catch {
        Write-Host "Error analyzing PE file for ${dllPath}" -ForegroundColor Red
        return "Error"
    }
}

# Function to detect managed (.NET) DLL architecture via CorFlags
function Get-ManagedDllArchitecture {
    param (
        [string]$dllPath
    )
    try {
        $corFlagsResult = & "corflags" $dllPath 2> $null
        if ($corFlagsResult -match "32BITREQ\s+:\s+1") {
            return "x86 (Managed)"
        } elseif ($corFlagsResult -match "PE\s+:\s+PE32\+") {
            return "x64 (Managed)"
        } elseif ($corFlagsResult -match "PE\s+:\s+PE32") {
            return "AnyCPU (Managed)"
        } else {
            return "Unknown (Managed)"
        }
    } catch {
        Write-Host "Error analyzing managed DLL via CorFlags for ${dllPath}" -ForegroundColor Red
        return "Error"
    }
}

# Prepare storage for results
$dllResults = @()

# Recursively process all DLLs in the target folder
Get-ChildItem -Path $targetFolder -Filter *.dll -Recurse | ForEach-Object {
    $dllPath = $_.FullName

    Write-Host "`nAnalyzing ${dllPath} ..." -ForegroundColor Yellow

    try {
        # Step 1: Check architecture via PE Headers
        $arch = Get-DllArchitecture -dllPath $dllPath

        # Step 2: Fallback for managed assemblies (e.g., AnyCPU)
        if ($arch -eq "Invalid PE file" -or $arch -eq "Unknown") {
            $arch = Get-ManagedDllArchitecture -dllPath $dllPath
        }

        # Step 3: Store the result
        $dllResults += [PSCustomObject]@{
            File         = $dllPath
            Architecture = $arch
        }

    } catch {
        Write-Host "Error analyzing ${dllPath}: $_" -ForegroundColor Red
    }
}

# Optional: Display results in a list to guarantee visibility
Write-Host "`n===== Results (Full List) =====" -ForegroundColor Cyan
$dllResults | Format-List

# Save results directly to a CSV file for analysis
$dllResults | Export-Csv -Path "DllResults.csv" -NoTypeInformation
Write-Host "`nResults saved to 'DllResults.csv'" -ForegroundColor Green