```markdown
# custom-os Development Patterns

> Auto-generated skill from repository analysis

## Overview
This skill teaches the core development patterns and conventions used in the `custom-os` TypeScript codebase. You'll learn about file organization, code style, import/export patterns, and how to write and run tests. This guide is ideal for contributors aiming to maintain consistency and quality in the project.

## Coding Conventions

### File Naming
- Use **camelCase** for file names.
  - Example: `userManager.ts`, `systemConfig.ts`

### Import Style
- Use **relative imports** for internal modules.
  - Example:
    ```typescript
    import { getUser } from './userManager';
    ```

### Export Style
- Use **named exports** rather than default exports.
  - Example:
    ```typescript
    // userManager.ts
    export function getUser(id: string) { ... }
    export function createUser(data: UserData) { ... }
    ```

### Commit Patterns
- Commit messages are **freeform** and do not follow a strict prefix or format.
  - Example: `add user authentication`

## Workflows

### Adding a New Module
**Trigger:** When you need to add a new feature or module to the codebase  
**Command:** `/add-module`

1. Create a new file using camelCase naming (e.g., `featureManager.ts`).
2. Implement your logic using TypeScript.
3. Use named exports for all functions and constants.
4. Import other modules using relative paths.
5. Write a corresponding test file named `featureManager.test.ts`.
6. Commit your changes with a descriptive message.

### Writing and Running Tests
**Trigger:** When you add or update code and need to verify correctness  
**Command:** `/test`

1. Create a test file following the `*.test.*` pattern (e.g., `userManager.test.ts`).
2. Write tests for your functions and features.
3. Use the project's test runner (framework not specified; check project scripts or documentation).
4. Run the tests and ensure all pass before committing.

## Testing Patterns

- Test files are named with the pattern `*.test.*` (e.g., `systemConfig.test.ts`).
- The testing framework is **unknown**; refer to project documentation or scripts for details.
- Place test files alongside the modules they test or in a dedicated test directory.
- Example test file structure:
  ```typescript
  // userManager.test.ts
  import { getUser } from './userManager';

  describe('getUser', () => {
    it('returns the correct user', () => {
      // test implementation
    });
  });
  ```

## Commands
| Command        | Purpose                                      |
|----------------|----------------------------------------------|
| /add-module    | Scaffold and add a new module to the project |
| /test          | Run all test files in the codebase           |
```
