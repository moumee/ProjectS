def find_last_good_version(relative_file_path):
    """
    Iterates through Git history for a file, checks each version,
    and automatically restores the first good version found.
    """
    
    absolute_file_path = os.path.join(os.getcwd(), relative_file_path).replace("\\", "/")
    print(f"--- Starting check for: {relative_file_path} ---")

    # 1. Get the commit history
    try:
        log_command = ["git", "log", "--pretty=format:%H", "--", relative_file_path]
        result = subprocess.run(log_command, capture_output=True, text=True, check=True)
        commit_hashes = result.stdout.splitlines()
    except subprocess.CalledProcessError as e:
        print(f"Error: Could not get git history for {relative_file_path}.")
        return

    if not commit_hashes:
        print(f"No git history found for {relative_file_path}.")
        return

    print(f"Found {len(commit_hashes)} commits. Checking from newest to oldest...")

    # 2. Get the current branch name to return to
    original_branch = subprocess.run(
        ["git", "rev-parse", "--abbrev-ref", "HEAD"], 
        capture_output=True, text=True
    ).stdout.strip()

    found_good_commit = None

    try:
        # 3. Iterate through commits to find a good one
        for i, commit_hash in enumerate(commit_hashes):
            print(f"\n[{i+1}/{len(commit_hashes)}] Checking commit: {commit_hash}")

            # 3a. Check out the file from that commit
            subprocess.run(
                ["git", "checkout", commit_hash, "--", relative_file_path],
                check=True, capture_output=True
            )
            # 3b. FORCE LFS to pull the data
            subprocess.run(
                ["git", "lfs", "pull", "--include", relative_file_path],
                check=True, capture_output=True
            )

            # 3c. Run the corruption check
            if check_asset_integrity(absolute_file_path):
                print(f"  [SUCCESS] Found uncorrupted version at commit: {commit_hash}")
                found_good_commit = commit_hash
                break  # Stop the loop, we found it
    
    finally:
        # 4. This block runs AFTER the loop (or if it errors out)
        # It handles either restoring the good file or cleaning up.
        
        if found_good_commit:
            # --- SUCCESS CASE ---
            # We found a good one, so check it out *for real* this time.
            print(f"\nRestoring file to working version from {found_good_commit}...")
            subprocess.run(
                ["git", "checkout", found_good_commit, "--", relative_file_path],
                check=True, capture_output=True
            )
            subprocess.run(
                ["git", "lfs", "pull", "--include", relative_file_path],
                check=True, capture_output=True
            )
            print("\n--- ✅ File Restored ---")
            print(f"'{relative_file_path}' has been replaced with the working version.")
            print("The file is now staged. You can `git commit` this fix.")
        
        else:
            # --- FAILURE CASE ---
            # No good version was found. Restore the file to its original
            # state (the HEAD of the branch) to clean up.
            print(f"\nRestoring '{relative_file_path}' to its state from branch '{original_branch}'...")
            subprocess.run(
                ["git", "checkout", original_branch, "--", relative_file_path], 
                check=True, capture_output=True
            )
            subprocess.run(
                ["git", "lfs", "pull", "--include", relative_file_path],
                check=True, capture_output=True
            )
            print("Restoration to original state complete.")

    # 5. Final status message
    if not found_good_commit:
        print(f"\n--- ❌ FAILURE ---")
        print(f"Checked all {len(commit_hashes)} versions. No uncorrupted version was found in history.")