//
//  OCShellAction.h
//  ShellActions.sugar
//
//  Created by Ian Beck on 2/23/12.
//  Copyright 2012 Ian Beck. MIT license.
//

#import <Foundation/Foundation.h>


/**
 * OCShellAction
 * 
 * OCShellAction allows users to hook shell scripts into the Espresso API,
 * similar to Textmate bundles. It can be used for both TextActions and
 * FileActions, although the environment variables and STDIN will vary
 * based on the type of action.
 * 
 * If OCShellAction is invoked as a TextAction and there are multiple
 * selections, it will iterate over all selections and invoke the script
 * once per selection (assuming the input is set to "selection"). This is
 * the ONLY time the script will be invoked multiple times for the same
 * action. FileActions with multiple selected files will simply have the
 * entire list of selected files passed in as a newline delimited list to
 * STDIN.
 * 
 * SETUP OPTIONS
 * 
 * Universal <setup> options:
 * - <script> (REQUIRED): you must specify the name of your script here. E.g.:
 * 
 *     <script>process_text.rb</script>
 * 
 *   Your script file will need to be saved in your Sugars's Scripts folder.
 * - <multiple-selections>: whether your script can handle multiple selections:
 *   - true (default)
 *   - false
 * - <single-selection>: whether your script can handle a single selection:
 *   - true (default)
 *   - false
 * - <empty-selection>: whether your script can handle an empty selection:
 *   - true (default)
 *   - false
 * - <suppress-errors>: whether script errors will be raised as exceptions or suppressed and logged
 *   - true (default)
 *   - false
 * - <error-output>: how the contents of STDERR will be output if suppress-errors is true
 *   - log (default): logged to Console.app
 *   - console: plain text in a new window
 *   - html: rendered as HTML in a new window
 *   - sheet: output in a sheet attached to the current window
 * - <config>: a plist dictionary containing optional script-specific configuration options
 * 
 * TextAction <setup> options:
 * - <input>: the contents of STDIN. Accepts:
 *   - selection (default)
 *   - document
 *   - nothing
 * - <alternate>: if your input is "selection", this is the fallback. Accepts:
 *   - document
 *   - line
 *   - word
 *   - character
 * - <output>: what your script will output. Accepts:
 *   - input (default): STDOUT will replace the input
 *   - document: STDOUT will replace the document
 *   - range: STDOUT represents one or more ranges to select
 *       * Ranges are formatted as 'location,length'. So first ten characters
 *         would be 0,10
 *       * Multiple ranges can be separated by linebreaks or &:
 *         0,10&12,5
 *   - tooltip: STDOUT will be displayed in a tooltip anchored to the selection (maximum 250 characters)
 *   - log: STDOUT will be output straight to the Console (using NSLog)
 *   - html: STDOUT will be rendered as HTML in a new window. Any relative links
 *     will resolve using EDITOR_SUGAR_PATH as the base URL
 *   - console: STDOUT will be displayed as plain text in a new window
 *   - nothing: STDOUT will be ignored
 * - <output-format>: the format that your script will output if overwriting text
 *   in the document. Accepts:
 *   - text (default): contents of STDOUT will be inserted as plain text.
 *   - snippet: contents of STDOUT will be inserted as a CETextSnippet.
 *     NOTE: if the user has multiple selections, your output
 *     will be automatically aggregated into a single snippet and overwrite the
 *     whole range. Make good use of the EDITOR_SELECTIONS_TOTAL and
 *     EDITOR_SELECTION_NUMBER environment variables to manage your tab stops!
 * 
 * ENVIRONMENT VARIABLES
 * 
 * Universal environment variables:
 * - EDITOR_SUGAR_PATH: the path to the root of the action's Sugar
 * - EDITOR_DIRECTORY_PATH: the path to the most specific possible context directory
 * - EDITOR_PROJECT_PATH: the path to the root project folder
 * - EDITOR_PATH: the path to the active file (only available in FileActions if
 *   there is only a single file)
 * - EDITOR_FILENAME: the filename of the active file (only available if EDITOR_PATH
     is set)
 * 
 * TextAction environment variables:
 * - EDITOR_CURRENT_WORD: the word around the cursor
 * - EDITOR_CURRENT_LINE: the line around the cursor
 * - EDITOR_LINE_INDEX: the zero-based index where the cursor falls in the line
 * - EDITOR_LINE_NUMBER: the number of the line around the cursor
 * - EDITOR_TAB_STRING: the string inserted when the user hits tab
 * - EDITOR_LINE_ENDING_STRING: the string inserted when the user hits enter
 * - EDITOR_ROOT_ZONE: textual ID of the root syntax zone
 * - EDITOR_ACTIVE_ZONE: textual ID of the active syntax zone
 * - EDITOR_SELECTIONS_TOTAL: the total number of selections in the document
 * - EDITOR_SELECTION_NUMBER: the number of the selection currently being processed
 * - EDITOR_SELECTION_RANGE: the range of the selected text in the document; uses
 *   the same formatting as the "range" output (index,length). So if the first ten
 *   characters are selected, this will be "0,10" (without the quotes)
 *
 * If the <config> dictionary is included, there will be environment variables
 * for each key with the prefix "CONFIG_". For instance, if this is in the XML:
 *
 *     <config>
 *         <dict>
 *             <key>favorite-food</key>
 *             <string>spaghetti</string>
 *         </dict>
 *     </config>
 *
 * Then there will be an environment variable named "CONFIG_favorite-food" with
 * the value "spaghetti".
 */

@interface OCShellAction : NSObject {
@private
    NSString *script;
	NSString *input;
	NSString *alternate;
	NSString *output;
	NSString *outputFormat;
	NSString *bundlePath;
	NSString *errorOutput;
	NSDictionary *configDict;
	BOOL allowMultipleSelections;
	BOOL allowSingleSelection;
	BOOL allowNoSelection;
	BOOL suppressErrors;
}

- (NSString *)findScript:(NSString *)fileName;
- (NSString *)runScriptWithInput:(NSString *)input;
- (void)processErrorsWithContext:(id)context;
- (NSArray *)parseRangesFromString:(NSString *)rangeString combineWithRangeValues:(NSArray *)rangeValues maxIndex:(NSUInteger)maxIndex;

@end
